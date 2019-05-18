cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(import2qufaya VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

option(QZ_ENABLE_TESTS "whether to enable unit tests" OFF)

set(SRCS 
    csv/section.hpp
    csv/section.cpp
    qufaya.hpp
    qufaya.cpp
)

add_executable(im2qufaya main.cpp ${SRCS})
target_compile_features(im2qufaya PRIVATE cxx_std_14)

# find_package(XLNT)
# target_link_libraries(im2qufaya ${XLNT_LIBRARIES})
# target_include_directories(im2qufaya PRIVATE ${XLNT_INCLUDE_DIRS})

if (QZ_ENABLE_TESTS)
    file(GLOB_RECURSE TEST_SRCS ./tests/*.cpp)
    add_executable(test_im2qufaya ${SRCS} ${TEST_SRCS})
    target_compile_features(test_im2qufaya PRIVATE cxx_std_14)

    add_subdirectory(ree_unittest)
    target_link_libraries(test_im2qufaya PRIVATE ree_unittest ${XLNT_LIBRARIES})
    target_include_directories(test_im2qufaya 
        PRIVATE ${XLNT_INCLUDE_DIRS}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif (QZ_ENABLE_TESTS)