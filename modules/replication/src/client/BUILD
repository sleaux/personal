cc_binary(
    name = "client",
    srcs = ["client_main.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//modules/replication/proto:caspaxos_cc",
        "@abseil-cpp//absl/flags:flag",
        "@abseil-cpp//absl/flags:parse",
        "@abseil-cpp//absl/flags:usage",
        "@abseil-cpp//absl/log",
        "@abseil-cpp//absl/log:initialize",
    ],
)
