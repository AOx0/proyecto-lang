const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const flags = .{
        "-Wall",
        "-Wextra",
        "-pedantic",
        "-W",
        "-Wstrict-prototypes",
        "-Wwrite-strings",
        "-Wno-missing-field-initializers",
    };

    const loxlib = b.addStaticLibrary(.{
        .name = "loxlib",
        .target = target,
        .optimize = optimize,
    });

    loxlib.linkLibC();
    loxlib.addCSourceFiles(&.{ "src/err.c", "src/str.c", "src/vector.c", "src/grammar.c", "src/hashset.c" }, &flags);

    {
        const lox = b.addExecutable(.{
            .name = "lox",
            .target = target,
            .optimize = optimize,
        });

        b.installArtifact(lox);
        lox.linkLibC();
        lox.linkLibrary(loxlib);
        lox.addCSourceFiles(&.{"src/main.c"}, &flags);

        const run_cmd = b.addRunArtifact(lox);
        run_cmd.step.dependOn(b.getInstallStep());

        if (b.args) |args| {
            run_cmd.addArgs(args);
        }

        const run_step = b.step("run", "Run the app");
        run_step.dependOn(&run_cmd.step);
    }

    {
        const lox = b.addExecutable(.{
            .name = "test",
            .target = target,
            .optimize = optimize,
        });

        b.installArtifact(lox);
        lox.linkLibC();
        lox.linkLibrary(loxlib);
        lox.addCSourceFiles(&.{ "test/main.c", "test/str_test.c", "test/vec_test.c" }, &flags);

        const run_cmd = b.addRunArtifact(lox);
        run_cmd.step.dependOn(b.getInstallStep());

        if (b.args) |args| {
            run_cmd.addArgs(args);
        }

        const run_step = b.step("test", "Run tests");
        run_step.dependOn(&run_cmd.step);
    }
}
