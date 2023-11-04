# lng

Final project for Theory of Programming Languages @ Universidad Panamericana

## Compilation

We use the [zig](https://ziglang.org/) compiler. Make sure to install the latest 
stable release (v0.11) [here](https://ziglang.org/download/#release-0.11.0) to be
able to compile the project.

To run the CLI application:

```
zig build run
```

To run all tests under `./test`

```
zig build test
```

To enable release, faster builds you can use Zig's flag `-Doptimize` with 
`ReleaseFast`, `ReleaseSafe`, etc. For example:

```
zig build -Doptimize=ReleaseFast run
```

For more help execute:

```
zig build --help
```
