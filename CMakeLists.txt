cmake_minimum_required(VERSION 3.25)

include(cmake/prelude.cmake)

project(
    tml
    VERSION 0.1.0
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(libtml
    source/tml.cpp
)
add_library(tml::tml ALIAS libtml)

include(GenerateExportHeader)
generate_export_header(
    libtml
    BASE_NAME tml
    EXPORT_FILE_NAME export/tml/config.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(libtml PUBLIC TML_STATIC_DEFINE)
endif()

set_target_properties(
    libtml PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME tml
    OUTPUT_NAME tml
)

target_include_directories(
    libtml ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    libtml SYSTEM
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(libtml PUBLIC cxx_std_20)

find_package(fmt REQUIRED)
target_link_libraries(libtml PRIVATE fmt::fmt)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT tml_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of tml"
  )
endif()

include(cmake/dev-mode.cmake)
