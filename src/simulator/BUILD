load("@bazel_rules_qt//:qt.bzl", "qt_cc_library")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "simulator_cli",
    srcs = ["simulator.cpp"],
    deps = [
        ":ssl_robocup_server",
        "//src/amun/simulator",
        "//src/config",
        "//src/core",
    ],
)

cc_library(
    name = "ssl_robocup_server",
    srcs = ["ssl_robocup_server.cpp"],
    hdrs = ["ssl_robocup_server.h"],
    deps = [
        "//src/amun/simulator",
        "@qt//:qt_network",
    ],
)
