CMAKE_MINIMUM_REQUIRED(VERSION 3.2.2)
PROJECT(gloost)

# c++11 standard
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX14)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -DBOOST_SYSTEM_DEPRACTED")
elseif(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DBOOST_SYSTEM_DEPRACTED")
else()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/lib)

# Needed packages
# Boost
if (UNIX)
  set (BOOST_ROOT "/opt/boost/current" CACHE PATH "Set to your boost install")
endif(UNIX)

FIND_PACKAGE(Boost REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR} )
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

#gloost
file(GLOB GLOOST_HEADER             		src/gloost/*.h)
file(GLOB GLOOST_SOURCE             		src/gloost/*.cpp)
file(GLOB GLOOST_ANIMATION_HEADER       	src/gloost/animation/*.h)
file(GLOB GLOOST_ANIMATION_SOURCE       	src/gloost/animation/*.cpp)
file(GLOB GLOOST_GL_HEADER       		src/gloost/gl/*.h)
file(GLOB GLOOST_GL_SOURCE       		src/gloost/gl/*.cpp)
file(GLOB GLOOST_HUMAN_INPUT_HEADER    src/gloost/human_input/*.h)
file(GLOB GLOOST_HUMAN_INPUT_SOURCE    src/gloost/human_input/*.cpp)
file(GLOB GLOOST_NETWORK_HEADER    	src/gloost/network/*.h)
file(GLOB GLOOST_NETWORK_SOURCE    	src/gloost/network/*.cpp)
file(GLOB GLOOST_LOG_HEADER    	src/gloost/log/*.h)
file(GLOB GLOOST_LOG_SOURCE    	src/gloost/log/*.cpp)
file(GLOB GLOOST_OBSERVER_HEADER    	src/gloost/log/observer/*.h)
file(GLOB GLOOST_OBSERVER_SOURCE    	src/gloost/log/observer/*.cpp)
file(GLOB GLOOST_UTIL_HEADER    	src/gloost/util/*.h)
file(GLOB GLOOST_UTIL_SOURCE    	src/gloost/util/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/contrib/tinyxml2/src)
include_directories(/usr/include/freetype2)
include_directories(/opt/zmq/latest/include)

ADD_LIBRARY(gloost STATIC
	${GLOOST_SOURCE} 			${GLOOST_HEADER}
	${GLOOST_ANIMATION_SOURCE}	${GLOOST_ANIMATION_HEADER}
	${GLOOST_GL_SOURCE}		${GLOOST_GL_HEADER}
	${GLOOST_HUMAN_INPUT_SOURCE}	${GLOOST_HUMAN_INPUT_HEADER}
	${GLOOST_NETWORK_SOURCE}	${GLOOST_NETWORK_HEADER}
	${GLOOST_LOG_SOURCE}	${GLOOST_LOG_HEADER}
	${GLOOST_OBSERVER_SOURCE}	${GLOOST_OBSERVER_HEADER}
	${GLOOST_UTIL_SOURCE}	${GLOOST_UTIL_HEADER}
)
TARGET_LINK_LIBRARIES(gloost GLEW GL ${Boost_LIBRARIES})
