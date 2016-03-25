# attempt to find static library first if this is set
if(YAMLCPP_USE_STATIC_LIBS)
    set(YAMLCPP_STATIC libyaml-cpp.a)
    set(YAMLCPP_STATIC_DEBUG libyaml-cpp-dbg.a)
endif()

# find the yaml-cpp include directory
find_path(YAMLCPP_INCLUDE_DIR
  NAMES yaml-cpp/yaml.h
  PATH_SUFFIXES include
  PATHS
    ~/Library/Frameworks/yaml-cpp/include/
    /Library/Frameworks/yaml-cpp/include/
    /usr/local/include/
    /usr/include/
    /sw/yaml-cpp/         # Fink
    /opt/local/yaml-cpp/  # DarwinPorts
    /opt/csw/yaml-cpp/    # Blastwave
    /opt/yaml-cpp/)

# find the release yaml-cpp library
find_library(YAMLCPP_LIBRARY_RELEASE
  NAMES ${YAMLCPP_STATIC} yaml-cpp libyaml-cppmd.lib
  PATH_SUFFIXES lib64 lib Release RelWithDebInfo
  PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt)

# find the debug yaml-cpp library
find_library(YAMLCPP_LIBRARY_DEBUG
  NAMES ${YAMLCPP_STATIC_DEBUG} yaml-cpp-dbg libyaml-cppmdd.lib
  PATH_SUFFIXES lib64 lib Debug
  PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt)

# set library vars
set(YAMLCPP_LIBRARY ${YAMLCPP_LIBRARY_RELEASE})
if(CMAKE_BUILD_TYPE MATCHES Debug AND EXISTS ${YAMLCPP_LIBRARY_DEBUG})
  set(YAMLCPP_LIBRARY ${YAMLCPP_LIBRARY_DEBUG})
endif()

get_filename_component(YAMLCPP_LIBRARY_RELEASE_DIR ${YAMLCPP_LIBRARY_RELEASE} PATH)
get_filename_component(YAMLCPP_LIBRARY_DEBUG_DIR ${YAMLCPP_LIBRARY_DEBUG} PATH)
set(YAMLCPP_LIBRARY_DIR ${YAMLCPP_LIBRARY_RELEASE_DIR} ${YAMLCPP_LIBRARY_DEBUG_DIR})

if(YAMLCPP_FIND_REQUIRED)
  set(DEP_FIND_FLAGS "${DEP_FIND_FLAGS} REQUIRED")
endif()
if(YAMLCPP_FIND_QUIETLY)
  set(DEP_FIND_FLAGS "${DEP_FIND_FLAGS} QUIETLY")
endif()
find_package(Boost ${DEP_FIND_FLAGS})

if(YAMLCPP_USE_STATIC_LIBS)
  add_library(YamlCpp STATIC IMPORTED)
else()
  add_library(YamlCpp SHARED IMPORTED)
endif()

set_target_properties(YamlCpp PROPERTIES INTERFACE_LINK_LIBRARIES Boost::boost)

if(EXISTS "${YAMLCPP_LIBRARY}")
  set_target_properties(YamlCpp PROPERTIES
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_LOCATION "${YAMLCPP_LIBRARY}")
endif()
if(EXISTS "${YAMLCPP_LIBRARY_DEBUG}")
  set_property(TARGET YamlCpp APPEND PROPERTY
    IMPORTED_CONFIGURATIONS DEBUG)
  set_target_properties(YamlCpp PROPERTIES
    IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
    IMPORTED_LOCATION_DEBUG "${YAMLCPP_LIBRARY_DEBUG}")
endif()
if(EXISTS "${YAMLCPP_LIBRARY_RELEASE}")
  set_property(TARGET YamlCpp APPEND PROPERTY
    IMPORTED_CONFIGURATIONS RELEASE)
  set_target_properties(YamlCpp PROPERTIES
    IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
    IMPORTED_LOCATION_RELEASE "${YAMLCPP_LIBRARY_RELEASE}")
endif()

# handle the QUIETLY and REQUIRED arguments and set YAMLCPP_FOUND to TRUE if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(YamlCpp DEFAULT_MSG
  YAMLCPP_INCLUDE_DIR
  YAMLCPP_LIBRARY
  YAMLCPP_LIBRARY_DIR)
mark_as_advanced(
  YAMLCPP_INCLUDE_DIR
  YAMLCPP_LIBRARY_DIR
  YAMLCPP_LIBRARY
  YAMLCPP_LIBRARY_RELEASE
  YAMLCPP_LIBRARY_RELEASE_DIR
  YAMLCPP_LIBRARY_DEBUG
  YAMLCPP_LIBRARY_DEBUG_DIR)
