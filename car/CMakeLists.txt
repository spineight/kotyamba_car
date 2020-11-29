cmake_minimum_required(VERSION 3.16)

project(car)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/car.hpp
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/car.cpp
)


add_library(car ${HEADERS} ${SOURCES})
add_library(kotyamba::car ALIAS car)
set_target_properties(car PROPERTIES LINKER_LANGUAGE CXX)

target_compile_features(car PUBLIC cxx_std_17)
target_include_directories(car PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(car PUBLIC kotyamba::motor_controller kotyamba::joystick)

add_executable(test_car ${CMAKE_CURRENT_SOURCE_DIR}/src/car.cpp)
set_target_properties(test_car PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(test_car PRIVATE car)


