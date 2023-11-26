const std = @import("std");
const Move = @import("build/move.zig").MoveFileStep;
const Repl = @import("build/replace.zig").ReplaceInFileStep;

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const no_bison = b.option(bool, "no-bison", "Do not run bison") orelse false;
    const no_flex = b.option(bool, "no-flex", "Do not run flex") orelse false;
    const no_gen = b.option(bool, "no-gen", "Do not run flex nor bison") orelse false;
    const debug_bison = b.option(bool, "debug-bison", "Run bison with the `-t` flag") orelse false;
    const debug_flex = b.option(bool, "debug-flex", "Run flex with the `-d` flag") orelse false;
    const debug_gen = b.option(bool, "debug-gen", "Run flex and bison with debug flags") orelse false;
    const table = b.option(bool, "table", "Print symbol table") orelse false;
    const tree = b.option(bool, "tree", "Print basic AST") orelse false;
    const werror = b.option(bool, "error", "Treat warnings as errors") orelse false;

    _ = std.fs.cwd().openFile("build.zig", .{}) catch {
        // Change current directory to where `build.zig` is.
        const build_path = b.build_root.path.?;
        std.log.info("Changing current working directory to: {s}", .{build_path});
        std.process.changeCurDir(build_path) catch {
            std.log.err("Failed to set cwd to {s}", .{build_path});
            return;
        };
    };

    // Gen bison & flex files
    const mv_flex = Move.create(b, &.{"./lex.yy.c"}, &.{"./src/lexer.c"});
    if (debug_gen or debug_flex) {
        mv_flex.step.dependOn(&b.addSystemCommand(&.{ "flex", "-d", "src/lexer.l" }).step);
    } else {
        mv_flex.step.dependOn(&b.addSystemCommand(&.{ "flex", "src/lexer.l" }).step);
    }

    const mv_bison = Move.create(b, &.{ "./grammar.tab.c", "./grammar.tab.h" }, &.{ "./src/parser.c", "./src/parser.h" });
    mv_bison.step.dependOn(&b.addSystemCommand(&.{ "bison", if (debug_gen or debug_bison) "-dt" else "-d", "src/grammar.y" }).step);

    const gen_step = b.step("gen", "Generar lexer.{c,h}, parser.{c, h}");
    gen_step.dependOn(&mv_flex.step);
    gen_step.dependOn(&mv_bison.step);

    const flags = .{ 
        "-Wall", "-Wextra", "-pedantic", "-Wno-missing-braces", 
        "-Wmissing-field-initializers", 
        if (werror) "-Werror" else "", 
        if (werror) "-pedantic-errors" else "", 
        if (target.isWindows()) "-DWIN" else "", 
        if (tree) "-DPRINT_TREE" else "" , 
        if (table) "-DPRINT_TABLE" else ""
    };
    // The main source code files without `main.c`. It is easier to compile it with specific main
    // files so that, for example, we can test it.
    const lnglib = b.addStaticLibrary(.{
        .name = "lnglib",
        .target = target,
        .optimize = optimize,
    });

    lnglib.linkLibC();
    lnglib.addCSourceFiles(&.{ "src/str.c", "src/vector.c", "src/hashset.c", "src/parser.c", "src/lexer.c", "src/symbol.c", "src/tree.c", "src/node.c", "src/panic.c" }, &flags);

    if (!no_flex and !no_gen) lnglib.step.dependOn(&mv_flex.step);
    if (!no_bison and !no_gen) {
        if (debug_bison or debug_gen) {
            const en_debug = Repl.create(b, &.{"src/parser.c"}, &.{ "int yydebug;", "grammar.tab.h" }, &.{ "int yydebug = 1;", "parser.h" });
            const en_debug2 = Repl.create(b, &.{"src/parser.c"}, &.{"extern int yydebug = 1;"}, &.{"extern int yydebug;"});
            en_debug.step.dependOn(&mv_bison.step);
            en_debug2.step.dependOn(&en_debug.step);
            lnglib.step.dependOn(&en_debug2.step);
        } else {
            const repl = Repl.create(b, &.{ "src/parser.c", "src/parser.h" }, &.{"grammar.tab.h"}, &.{"parser.h"});
            repl.step.dependOn(&mv_bison.step);
            lnglib.step.dependOn(&repl.step);
        }
    }

    {
        const lng = b.addExecutable(.{
            .name = "lng",
            .target = target,
            .optimize = optimize,
        });

        b.installArtifact(lng);
        lng.linkLibC();
        lng.linkLibrary(lnglib);
        lng.addCSourceFiles(&.{"src/main.c"}, &flags);

        const run_cmd = b.addRunArtifact(lng);
        run_cmd.step.dependOn(b.getInstallStep());

        if (b.args) |args| {
            run_cmd.addArgs(args);
        }

        const run_step = b.step("run", "Run the app");
        run_step.dependOn(&run_cmd.step);
    }

    {
        const lng = b.addExecutable(.{
            .name = "test",
            .target = target,
            .optimize = optimize,
        });

        b.installArtifact(lng);
        lng.linkLibC();
        lng.linkLibrary(lnglib);
        lng.addCSourceFiles(&.{ "test/main.c", "test/str_test.c", "test/vec_test.c", "test/hash_test.c", "test/tree_test.c" }, &.{});

        const run_cmd = b.addRunArtifact(lng);
        run_cmd.step.dependOn(b.getInstallStep());
        const run_step = b.step("test", "Run tests");
        run_step.dependOn(&run_cmd.step);
    }
}
