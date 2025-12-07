# Bazel rules for the P programming language

## Overview
This repository provides bazel rules for the [P programming language](https://github.com/p-org/P).

## Quick start
Requirements:
* Bazel 7.0.0 or later
* Bzlmod enabled

Add this to your `MODULE.bazel` file:
```starlark
bazel_dep(name = "rules_p", version = "0.0.1")

p = use_extension("@rules_p//p:extensions.bzl", "p")
p.toolchain(p_version = "3.0.0")
use_repo(p, "p_toolchains")
register_toolchains("@p_toolchains//:all")
```

Add to a `BUILD.bazel` file:
```starlark
load("@rules_p//p:defs.bzl", "p_project", "p_test")

p_project(
    name = "my-project",
    srcs = glob(["**/*.p"]),
)

p_test(
    name = "testcase-1",
    model = ":my-project",
    testcase = "testcase-1",
)
```

## Documentation
For detailed documentation, see [docs/](./docs)

## Known issues
* A `p_test` rule that references a `testcase` that does not exist will silently pass.
