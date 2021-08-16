load("@bazel_rules_qt//:qt.bzl", "qt_cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "simulator",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.h"]),
    linkstatic = True,
    deps = [
        "//src/core",
        "//src/protobuf",
        ":simulator_qt",
    ],
)

qt_cc_library(
    name = "simulator_qt",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.h"]),
    linkstatic = True,
    deps = [
        "//src/core",
        "//src/protobuf",
        "@bullet",
        "@qt//:qt_core",
        "@qt//:qt_gui",
        "@qt//:qt_widgets",
    ],
)
