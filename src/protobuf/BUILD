package(default_visibility = ["//visibility:public"])

load("@rules_proto//proto:defs.bzl", "proto_library")

proto_library(
    name = "protobuf_proto_lib",
    srcs = glob([
        "*.proto",
    ]),
    visibility = ["//visibility:private"],
    deps = [
        "@com_google_protobuf//:any_proto",
        "@com_google_protobuf//:duration_proto",
        "@com_google_protobuf//:timestamp_proto",
    ],
)

cc_proto_library(
    name = "protobuf_cc_lib",
    deps = [":protobuf_proto_lib"],
)

cc_library(
    name = "protobuf",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["include/protobuf/*.h"]),
    strip_include_prefix = "include/protobuf/",
    deps = [
        ":protobuf_cc_lib",
        "@qt//:qt_core",
        "@qt//:qt_gui",
        "@qt//:qt_widgets",
    ],
)

cc_library(
    name = "protobuf_includes_for_external",
    hdrs = glob(["include/protobuf/*.h"]),
    strip_include_prefix = "include/",
)
