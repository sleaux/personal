load("@rules_zig//zig:defs.bzl", "zig_binary", "zig_configure", "zig_configure_binary", "zig_shared_library", "zig_static_library", "zig_test")
load("@rules_zig//zig:toolchain.bzl", "zig_target_toolchain")

constraint_setting(
    name = "wasm_setting",
)

constraint_value(
    name = "wasm",
    constraint_setting = ":wasm_setting",
)

zig_target_toolchain(
    name = "wasm32-freestanding-target",
    target = "wasm32-freestanding",
)

toolchain(
    name = "wasm32-freestanding-toolchain",
    target_compatible_with = [
        "@platforms//os:linux",
        ":wasm",
    ],
    toolchain = ":wasm32-freestanding-target",
    toolchain_type = "@rules_zig//zig/target:toolchain_type",
)

platform(
    name = "wasm32-freestanding",
    constraint_values = [
        "@platforms//os:linux",
        ":wasm",
    ],
)

zig_binary(
    name = "main",
    main = "main.zig",
)

zig_binary(
    name = "lib",
    main = "lib.zig",
    tags = ["manual"],
    zigopts = ["-fno-entry"],
)

zig_test(
    name = "lib_test",
    main = "lib.zig",
)

zig_configure_binary(
    name = "lib_wasm",
    actual = ":lib",
    target = ":wasm32-freestanding",
    zigopt = [
        "--export=add_two",
        "-O",
        "ReleaseFast",
    ],
)
