package(default_visibility = ["//visibility:public"])

cc_library(
    name = "simulator",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.h"]),
    #strip_include_prefix = "include/simulator/",
    deps = [
        ":simulator_includes",
        "//src/core",
        "//src/protobuf",
        "//src/protobuf:protobuf_includes_for_external",
        "@bullet",
        "@qt//:qt_core",
        "@qt//:qt_gui",
        "@qt//:qt_widgets",
    ],
)

cc_library(
    name = "simulator_includes",
    hdrs = glob(["include/simulator/*.h"]),
    strip_include_prefix = "include/simulator/",
    deps = [
        "//src/core:core_includes_for_external",
    ],
)
