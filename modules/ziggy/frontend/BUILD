load("@aspect_bazel_lib//lib:copy_to_directory.bzl", "copy_to_directory")
load("@aspect_rules_js//js:defs.bzl", "js_binary", "js_library", "js_run_devserver")
load("@rules_multirun//:defs.bzl", "command", "multirun")
load("@npm//:defs.bzl", "npm_link_all_packages")
load("@aspect_rules_js//js:defs.bzl", "js_binary")

js_binary(
    name = "server",
    data = [
        "//:node_modules/express",
        "//:node_modules/string-replace-middleware",
    ] + glob([
        "*.js",
        "public/*",
    ]),
    entry_point = "index.js",
    tags = [
        "ibazel_live_reload",
        "ibazel_notify_changes",
    ],
)
