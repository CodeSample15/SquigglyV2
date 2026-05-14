cmake_minimum_required(VERSION 3.28)

PROJECT (scribble_tests LANGUAGES CXX)

add_compile_options(-Wall -Werror)
file( GLOB_RECURSE TESTS "./*.cpp" )
file( GLOB_RECURSE SCRIBBLE "../src/*.cpp")

add_executable( scribble_tests ${TESTS} )

include_directories( ../include )
add_library( scribble_tests )