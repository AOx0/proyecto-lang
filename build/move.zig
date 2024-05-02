const std = @import("std");

pub const MoveFileStep = struct {
    const Self = @This();
    step: std.Build.Step,
    from: []const []const u8,
    into: []const []const u8,

    pub fn create(b: *std.Build, from: []const []const u8, into: []const []const u8) *MoveFileStep {
        const self = b.allocator.create(Self) catch @panic("out of memory");
        self.* = Self{ .step = std.Build.Step.init(.{
            .id = .custom,
            .name = "Move step",
            .owner = b,
            .makeFn = make,
        }), .from = from, .into = into };
        return self;
    }

    pub fn make(step: *std.Build.Step, prog_node: *std.Progress.Node) !void {
        _ = prog_node;
        const self: *MoveFileStep = @fieldParentPtr("step", step);

        for (self.from, self.into) |from, into| {
            std.fs.cwd().rename(from, into) catch {
                std.log.err("Failed to move {s} to {s}", .{ self.from, self.into });
            };
        }
    }
};
