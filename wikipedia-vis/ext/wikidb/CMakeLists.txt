cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(PROJECT_NAME_STR wikidb)

project(${PROJECT_NAME_STR} C CXX)

file(GLOB wikidb_sources . src/*.cpp)
# Boost
if (UNIX)
  set (BOOST_ROOT "/opt/boost/current" CACHE PATH "Set to your boost install")
endif(UNIX)
find_package( Boost COMPONENTS filesystem system regex serialization REQUIRED)
find_package( Threads REQUIRED )

# link to fastdb library
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}
  ${PROJECT_SOURCE_DIR}/contrib/lib)

add_definitions(-DHAVE_CONFIG_H)
find_library(FASTDB_LIBRARY NAMES libfastdb.a)

set(FASTDB_LIBRARY_WHOLE -Wl,-whole-archive ${FASTDB_LIBRARY} -Wl,-no-whole-archive)

# our compiler options:
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/lib)
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_BUILD_TYPE Debug)

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/contrib/include/fastdb")
include_directories(${Boost_INCLUDE_DIR})

# enable generation of profiling information during compilation and linking
# only for DEBUG build
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pg")
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -pg")
# SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} -pg")

add_library(wikidb STATIC ${wikidb_sources})
target_link_libraries(wikidb
  ${FASTDB_LIBRARY})

# small cpp file for testing wikidb
add_executable(db_test ${PROJECT_SOURCE_DIR}/tests/wikidbtest.cpp)
target_link_libraries(db_test wikidb ${CMAKE_THREAD_LIBS_INIT})

include_directories("${PROJECT_SOURCE_DIR}/contrib/docopt")
set(DOCOPT ${PROJECT_SOURCE_DIR}/contrib/docopt/docopt.cpp)

add_executable(createdb createdb.cpp ${DOCOPT})
target_link_libraries(createdb ${Boost_LIBRARIES}
  wikidb
  ${CMAKE_THREAD_LIBS_INIT})


# add testing
enable_testing()

set(GTEST_ROOT ${PROJECT_SOURCE_DIR}/contrib)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/tests/*.cpp)
add_executable(wikidb_test ${TEST_SRC_FILES})
target_link_libraries(wikidb_test
	${GTEST_BOTH_LIBRARIES}
	${Boost_LIBRARIES}
	wikidb
	${CMAKE_THREAD_LIBS_INIT})

GTEST_ADD_TESTS(wikidb_test "" ${TEST_SRC_FILES})
