load("@bazel_rules_qt//:qt.bzl", "qt_cc_library")

package(default_visibility = ["//visibility:public"])

qt_cc_library(
    name = "core",
    srcs = glob(
        ["*.cpp"],
        exclude = ["timer_main.cpp"],
    ),
    hdrs = glob(["*.h"]),
    linkstatic = True,
    deps = [
        "@qt//:qt_core",
        "@qt//:qt_gui",
        "@qt//:qt_widgets",
    ],
)

cc_library(
    name = "timer",
    srcs = ["timer.cpp"],
    hdrs = ["timer.h"],
    deps = [
        "@qt//:qt_core",
        "@qt//:qt_gui",
        "@qt//:qt_widgets",
    ],
    #linkstatic = True,
)

cc_binary(
    name = "timer_main",
    srcs = ["timer_main.cpp"],
    deps = [
        "timer",
    ],
)
