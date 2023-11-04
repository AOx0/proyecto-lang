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

    const lnglib = b.addStaticLibrary(.{
        .name = "lnglib",
        .target = target,
        .optimize = optimize,
    });

    lnglib.linkLibC();
    lnglib.addCSourceFiles(&.{ "src/str.c", "src/vector.c", "src/grammar.c", "src/hashset.c" }, &flags);

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
        lng.addCSourceFiles(&.{ "test/main.c", "test/str_test.c", "test/vec_test.c" }, &flags);

        const run_cmd = b.addRunArtifact(lng);
        run_cmd.step.dependOn(b.getInstallStep());

        if (b.args) |args| {
            run_cmd.addArgs(args);
        }

        const run_step = b.step("test", "Run tests");
        run_step.dependOn(&run_cmd.step);
    }
}
