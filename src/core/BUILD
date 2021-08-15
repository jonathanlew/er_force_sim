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
    linkstatic = True,
)

cc_library(
    name = "timer",
    srcs = ["timer.cpp"],
    hdrs = ["include/core/timer.h"],
    strip_include_prefix = "include/core/",
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

cc_library(
    name = "core_includes_for_external",
    hdrs = glob(["include/core/*.h"]),
    strip_include_prefix = "include/",
    deps = ["core"],
    linkstatic = True,
)
