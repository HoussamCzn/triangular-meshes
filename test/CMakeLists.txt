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
  source/tml_test.cpp
)
target_link_libraries(
    tml_test PRIVATE
    tml::tml
    Catch2::Catch2WithMain
)
target_compile_features(tml_test PRIVATE cxx_std_20)

catch_discover_tests(tml_test)

# ---- End-of-file commands ----

add_folders(Test)
