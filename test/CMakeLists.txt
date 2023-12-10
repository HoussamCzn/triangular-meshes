cmake_minimum_required(VERSION 3.25)

project(tmlTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(tml REQUIRED)
  enable_testing()
endif()

find_package(Catch2 REQUIRED)
include(Catch)

# ---- Tests ----

add_executable(tml_test
    source/face.test.cpp
    source/mesh.test.cpp
    source/vec3.test.cpp
    source/vertex.test.cpp
)
target_link_libraries(
    tml_test PRIVATE
    tml::tml
    Catch2::Catch2WithMain
)
target_compile_features(tml_test PRIVATE cxx_std_20)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/input.ply
    "ply\n"
    "format ascii 1.0\n"
    "comment zipper output\n"
    "element vertex 8\n"
    "property float x\n"
    "property float y\n"
    "property float z\n"
    "element face 12\n"
    "property list uchar int vertex_indices\n"
    "end_header\n"
    "-1 -1 -1\n"
    "1 -1 -1\n"
    "-1 1 -1\n"
    "1 1 -1\n"
    "-1 -1 1\n"
    "1 -1 1\n"
    "-1 1 1\n"
    "1 1 1\n"
    "3 3 1 0\n"
    "3 2 3 0\n"
    "3 3 7 1\n"
    "3 5 1 7\n"
    "3 6 5 7\n"
    "3 5 6 4\n"
    "3 4 6 2\n"
    "3 4 2 0\n"
    "3 6 7 3\n"
    "3 3 2 6\n"
    "3 1 5 0\n"
    "3 4 0 5\n"
)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/uncentered_input.ply
    "ply\n"
    "format ascii 1.0\n"
    "comment original uncentered mesh\n"
    "element vertex 8\n"
    "property float x\n"
    "property float y\n"
    "property float z\n"
    "end_header\n"
    "0 0 0\n"
    "2 0 0\n"
    "0 2 0\n"
    "2 2 0\n"
    "0 0 2\n"
    "2 0 2\n"
    "0 2 2\n"
    "2 2 2\n"
)

catch_discover_tests(tml_test)

# ---- End-of-file commands ----

add_folders(Test)
