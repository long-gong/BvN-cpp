#
# Tests
cmake_minimum_required( VERSION 3.0 )

project( BvNTests CXX )

message( STATUS "Tests enabled" )

# multiple-file modules:

set( SRCs
    test_main.cc test_stuffing.cc
)

add_executable(${PROJECT_NAME} ../matching_wrapper.hpp
        ${SRCs})

target_link_libraries(${PROJECT_NAME}
        armadillo)