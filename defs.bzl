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
    "-Wno-unused-parameter",
    "-fcoroutines-ts",
    "-std=c++2a",
]

DARWIN_TEST_COVERAGE_COPTS = [
    "-fprofile-arcs",
    "-ftest-coverage",
    "-fprofile-instr-generate", 
    "-fcoverage-mapping",
    "-ftest-coverage",
]

DARWIN_TEST_COVERAGE_LINKOPTS = [
    "-fprofile-arcs",
    "-ftest-coverage",
    "-fprofile-instr-generate",
]
