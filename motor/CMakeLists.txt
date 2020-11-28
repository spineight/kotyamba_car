cmake_minimum_required(VERSION 3.16)

project(motor)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../extern/pigpio/util")

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

# http://abyz.me.uk/rpi/pigpio/index.html
option(WITH_WIRINGPI "For controlling GPIO on Raspberry use WiringPi library" OFF)
# http://wiringpi.com/reference/core-functions/
option(WITH_PIGPIO "For controlling GPIO on Raspberry use pigpio library" ON)

if ((WITH_WIRINGPI AND WITH_PIGPIO) OR (NOT WITH_WIRINGPI AND NOT WITH_PIGPIO))
    message(FATAL "ONLY one gpio library should be ON. Set WITH_WIRINGPI or WITH_PIGPIO to ON")
elseif(WITH_WIRINGPI)
    # Locate libraries and headers
    find_package(WiringPi REQUIRED)
    target_include_directories(motor PUBLIC ${WIRINGPI_INCLUDE_DIRS})
    target_link_libraries(motor PUBLIC ${WIRINGPI_LIBRARIES} crypt)
    target_compile_definitions(motor PUBLIC WITH_WIRINGPI=1)
elseif(WITH_PIGPIO)
    # https://github.com/joan2937/pigpio/blob/master/util/Findpigpio.cmake
    find_package(pigpio REQUIRED)
    target_link_libraries(motor PUBLIC ${pigpio_LIBRARY})
    target_include_directories(motor PUBLIC ${pigpio_INCLUDE_DIR})
    target_compile_definitions(motor PUBLIC WITH_PIGPIO=1)
endif ()

find_package(Threads REQUIRED)
target_link_libraries(motor PUBLIC ${CMAKE_THREAD_LIBS_INIT} rt)



add_executable(test_motor ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
set_target_properties(test_motor PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(test_motor PRIVATE kotyamba::motor ncurses)

