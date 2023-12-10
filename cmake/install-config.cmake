include(CMakeFindDependencyMacro)
find_dependency(fmt)
find_dependency(pugixml)

include("${CMAKE_CURRENT_LIST_DIR}/tmlTargets.cmake")
