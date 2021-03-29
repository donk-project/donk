WINDOWS_DEFAULT_COPTS = [
    "/analyze:WX-",
    "/analyze:max_paths 512",
    "/await",
    "/std:c++17",
]

DARWIN_DEFAULT_COPTS = [
    "-Wall",
    "-Wcast-align",
    "-Wextra",
    "-Wfloat-equal",
    "-Wmissing-declarations",
    "-Woverloaded-virtual",
    "-Wredundant-decls",
    "-Wshadow",
    "-Wundef",
    "-Wwrite-strings",
    "-fcoroutines-ts",
    "-std=c++2a",
]
