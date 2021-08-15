load("@rules_proto//proto:defs.bzl", "proto_library")

proto_library(
    name = "protobuf_lib",
    srcs = glob([
        "*.proto",
    ]),
    visibility = ["//visibility:private"],
    deps = [
        "@com_google_protobuf//:any_proto",
        "@com_google_protobuf//:timestamp_proto",
        "@com_google_protobuf//:duration_proto",
    ],
)

cc_proto_library(
    name = "protobuf_cc_lib",
    deps = [":protobuf_lib"],
)
