package(default_visibility = ["//visibility:public"])

genrule(
    name = "generate_config",
    srcs = ["include/config/config.h.in"],
    outs = ["config.h"],
    cmd = "\n".join([
        "cp src/config/include/config/config.h.in $@",
    ]),
)

cc_library(
    name = "config",
    hdrs = ["config.h"],
    linkstatic = True,
)
