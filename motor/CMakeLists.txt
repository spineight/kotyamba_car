cmake_minimum_required(VERSION 3.16)

project(motor)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/motor.hpp
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/motor.cpp
)


add_library(motor ${HEADERS} ${SOURCES})
add_library(kotyamba::motor ALIAS motor)
set_target_properties(motor PROPERTIES LINKER_LANGUAGE CXX)

target_compile_features(motor PUBLIC cxx_std_17)
target_include_directories(motor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Locate libraries and headers
find_package(WiringPi REQUIRED)
find_package(Threads REQUIRED)

target_include_directories(motor PUBLIC ${WIRINGPI_INCLUDE_DIRS})
target_link_libraries(motor PUBLIC ${WIRINGPI_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} rt crypt)

add_executable(test_motor ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
set_target_properties(test_motor PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(test_motor PRIVATE kotyamba::motor ncurses)

