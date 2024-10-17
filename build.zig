const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "jit-compiler-brainfuck",
        .target = target,
        .optimize = optimize,
    });

    exe.addCSourceFiles(.{
        .files = &.{"src/main.cpp"},
        .flags = &.{"-std=c++20"},
    });

    exe.linkLibCpp();

    b.installArtifact(exe);

    const runCMD = b.addRunArtifact(exe);
    runCMD.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        runCMD.addArgs(args);
    }

    const runStep = b.step("run", "Run the application");
    runStep.dependOn(&runCMD.step);
}
