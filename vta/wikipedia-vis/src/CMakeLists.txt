file(GLOB VTA_SOURCE *.cpp)
file(GLOB VTA_HEADER *.hpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_library(vta STATIC ${VTA_SOURCE} ${VTA_HEADER})
