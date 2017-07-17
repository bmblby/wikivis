cmake_minimum_required (VERSION 3.2.2)
project(wiki-vis)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX14)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wno-deprecated-declarations -DBOOST_SYSTEM_DEPRACTED")
elseif(COMPILER_SUPPORTS_CXX11)
  #TODO ignore warning of depracted std::binder2nd dependencies of eigen, wait for upgrade to eigen 3.3
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated-declarations -DBOOST_SYSTEM_DEPRACTED")
else()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# Boost
if (UNIX)
  set (BOOST_ROOT "/opt/boost/current" CACHE PATH "Set to your boost install")
endif(UNIX)

find_package(Boost COMPONENTS
  timer
  serialization
  regex
  system
  filesystem
  graph
  REQUIRED
)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

#glm
include_directories(/opt/glm)

# GLFW
set(GLFW_INSTALL OFF CACHE STRING "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE STRING "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE STRING "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE STRING "" FORCE)

#nanogui
set(NANOGUI_BUILD_EXAMPLE OFF CACHE BOOL " " FORCE)
set(NANOGUI_BUILD_PYTHON OFF CACHE BOOL " " FORCE)
set(NANOGUI_INSTALL OFF CACHE BOOL " " FORCE)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ext/nanogui)
set_property(TARGET glfw glfw_objects nanogui PROPERTY FOLDER "dependencies")
include_directories(${CMAKE_SOURCE_DIR}/ext/nanogui/include)

# nanovg
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ext/nanovg)

# gloost
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ext/gloost)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/gloost/contrib/tinyxml2/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/gloost/src)

# wikidb
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ext/wikidb)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/wikidb/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/wikidb/contrib/include/fastdb)

include_directories(/usr/include/freetype2)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# libvta_static
add_subdirectory(src)
add_subdirectory(app)
