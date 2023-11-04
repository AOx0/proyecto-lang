const std = @import("std");
const Move = @import("build/move.zig").MoveFileStep;

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    // Change current directory to where `build.zig` is.
    const build_path = b.build_root.path.?;
    std.log.info("Changing current working directory to: {s}", .{build_path});
    std.process.changeCurDir(build_path) catch {
        std.log.err("Failed to set cwd to {s}", .{build_path});
        return;
    };

    const flags = .{ "-Wall", "-Wextra", "-Werror", "-pedantic", "-pedantic-errors", if (target.isWindows()) "-DWIN" else "" };

    // The main source code files without `main.c`. It is easier to compile it with specific main
    // files so that, for example, we can test it.
    const lnglib = b.addStaticLibrary(.{
        .name = "lnglib",
        .target = target,
        .optimize = optimize,
    });

    // Gen bison & flex files
    const mv_flex = Move.create(b, &.{"./lex.yy.c"}, &.{"./src/lexer.c"});
    mv_flex.step.dependOn(&b.addSystemCommand(&.{ "flex", "-L", "src/lexer.l" }).step);

    const mv_bison = Move.create(b, &.{ "./grammar.tab.c", "./grammar.tab.h" }, &.{ "./src/parser.c", "./src/include/parser.h" });
    mv_bison.step.dependOn(&b.addSystemCommand(&.{ "bison", "-ld", "src/grammar.y" }).step);

    const gen_step = b.step("gen", "Generar lexer.{c,h}, parser.{c, h}");
    gen_step.dependOn(&mv_flex.step);
    gen_step.dependOn(&mv_bison.step);

    lnglib.linkLibC();
    lnglib.addCSourceFiles(&.{ "src/str.c", "src/vector.c", "src/hashset.c", "src/parser.c", "src/lexer.c" }, &flags);

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
        lng.step.dependOn(gen_step);

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

        lng.linkLibC();
        lng.linkLibrary(lnglib);
        lng.addCSourceFiles(&.{ "test/main.c", "test/str_test.c", "test/vec_test.c" }, &flags);
        lng.step.dependOn(gen_step);

        const run_cmd = b.addRunArtifact(lng);
        run_cmd.step.dependOn(b.getInstallStep());
        const run_step = b.step("test", "Run tests");
        run_step.dependOn(&run_cmd.step);
    }
}
