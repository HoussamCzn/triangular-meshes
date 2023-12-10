if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/tml-${PROJECT_VERSION}"
      CACHE STRING ""
  )
  set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package tml)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT tml_Development
)

install(
    TARGETS libtml
    EXPORT tmlTargets
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    tml_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
set_property(CACHE tml_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(tml_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${tml_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT tml_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${tml_INSTALL_CMAKEDIR}"
    COMPONENT tml_Development
)

install(
    EXPORT tmlTargets
    NAMESPACE tml::
    DESTINATION "${tml_INSTALL_CMAKEDIR}"
    COMPONENT tml_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
