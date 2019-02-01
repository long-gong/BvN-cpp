cmake_minimum_required(VERSION 3.0)

project(BvN)
set(CMAKE_CXX_STANDARD 17)

enable_testing()
add_definitions(-DDEBUG)
#add_executable(${PROJECT_NAME} test.cc)
#target_link_libraries(${PROJECT_NAME} armadillo)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include/third_party)
add_subdirectory(test)

