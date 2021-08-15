package(default_visibility = ["//visibility:public"])

cc_library(
    name = "core",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["include/core/*.h"]),
    strip_include_prefix = "include/core/",
    deps = [
        "@qt//:qt_core",
        "@qt//:qt_gui",
        "@qt//:qt_widgets",
    ],
)
