include_directories(${NANOGUI_EXTRA_INCS})
add_executable(app main.cpp)

set(FASTDB_LIBRARY_SEARCH_DIR
  ${CMAKE_SOURCE_DIR}/ext/wikidb/contrib/lib
)
find_library(fastdb NAMES libfastdb.so
                    PATHS ${FASTDB_LIBRARY_SEARCH_DIR})

target_link_libraries(app
                      vta
                      m
                      gloost
                      nanogui
                      nanovg
                      ${NANOGUI_EXTRA_LIBS}
                      GLEW
                      GL
                      wikidb
                      ${fastdb}
                      freeimage
                      freetype
                      ${Boost_LIBRARIES}
)
add_dependencies(app vta wikidb gloost)
