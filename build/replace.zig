const std = @import("std");

pub const ReplaceInFileStep = struct {
    const Self = @This();
    step: std.build.Step,
    from: []const []const u8,
    into: []const []const u8,
    paths: []const []const u8,

    pub fn create(b: *std.build.Builder, paths: []const []const u8, from: []const []const u8, into: []const []const u8) *ReplaceInFileStep {
        const self = b.allocator.create(Self) catch @panic("out of memory");
        self.* = Self{ .step = std.build.Step.init(.{
            .id = .custom,
            .name = "Replace step",
            .owner = b,
            .makeFn = make,
        }), .from = from, .into = into, .paths = paths };
        return self;
    }

    pub fn make(step: *std.build.Step, prog_node: *std.Progress.Node) !void {
        _ = prog_node;
        const self = @fieldParentPtr(ReplaceInFileStep, "step", step);

        for (self.paths) |path| {
            var file = try std.fs.cwd().openFile(path, .{ .mode = .read_only });

            const file_size = (try file.stat()).size;
            var buffer = try self.step.owner.allocator.alloc(u8, file_size);
            defer self.step.owner.allocator.free(buffer);

            var buffer2 = try self.step.owner.allocator.alloc(u8, file_size * 2);
            defer self.step.owner.allocator.free(buffer2);

            try file.reader().readNoEof(buffer);
            file.close();

            const n = replace(buffer, buffer2, self.from, self.into);

            file = try std.fs.cwd().createFile(path, .{ .truncate = true });
            defer file.close();

            try file.writeAll(buffer2[0..n]);
        }
    }
};

pub fn contains(inp: []const u8, substr: []const u8) bool {
    var i: usize = 0;

    while (i < inp.len) {
        if (inp[i..].len >= substr.len and std.mem.eql(u8, inp[i .. i + substr.len], substr)) {
            return true;
        }
        i += 1;
    }

    return false;
}

const StrTupla = struct {
    left: []const u8,
    right: []const u8,
};

pub fn split_once(inp: []const u8, substr: []const u8) ?StrTupla {
    var i: usize = 0;

    while (i < inp.len) {
        if (inp.len >= i + substr.len and std.mem.eql(u8, inp[i .. i + substr.len], substr)) {
            var res: StrTupla = .{ .left = inp[0..i], .right = inp[i + substr.len ..] };
            return res;
        }

        i += 1;
    }

    return null;
}

pub fn replace(inp: []const u8, buffer: []u8, from: []const []const u8, into: []const []const u8) usize {
    var i: usize = 0;
    var j: usize = 0;

    while (i < inp.len) {
        var matched = false;

        for (0..from.len) |n_rule| {
            const repl = from[n_rule];
            const to = into[n_rule];

            if (inp[i..].len >= repl.len and std.mem.eql(u8, inp[i .. i + repl.len], repl)) {
                @memcpy(buffer[j .. j + to.len], to);
                j += to.len;
                i += repl.len;
                matched = true;
            } else if (contains(repl, "@@")) {
                const repl_split = split_once(repl, "@@").?;
                const repl_start: []const u8 = repl_split.left;
                const repl_end: []const u8 = repl_split.right;

                if (!(inp[i..].len >= repl_start.len)) continue;
                if (!contains(inp[i + repl_start.len ..], repl_end)) continue;

                const to_split = split_once(to, "@@").?;
                const to_start: []const u8 = to_split.left;
                const to_end: []const u8 = to_split.right;

                if (inp[i..].len >= repl_start.len and std.mem.eql(u8, inp[i .. i + repl_start.len], repl_start)) {
                    @memcpy(buffer[j .. j + to_start.len], to_start);
                    j += to_start.len;
                    i += repl_start.len;
                    while (!std.mem.eql(u8, inp[i .. i + repl_end.len], repl_end)) {
                        buffer[j] = inp[i];
                        j += 1;
                        i += 1;
                    }
                    @memcpy(buffer[j .. j + to_end.len], to_end);
                    j += to_end.len;
                    i += repl_end.len;
                    matched = true;
                }
            }
        }

        if (!matched) {
            buffer[j] = inp[i];
            j += 1;
            i += 1;
        }
    }

    return j;
}
