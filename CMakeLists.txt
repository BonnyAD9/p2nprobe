# Author: Jakub Antonín Štigler (xstigl00)

cmake_minimum_required(VERSION 3.16)

project(p2nprobe VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(p2nprobe)

file(GLOB p2nprobe_files CONFIGURE_DEPENDS "src/**/*.cpp" "src/*.cpp")
target_sources(p2nprobe PRIVATE ${p2nprobe_files})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -fsanitize=address")
    target_link_libraries(p2nprobe PRIVATE -fsanitize=address)
endif()

target_link_libraries(p2nprobe PRIVATE
    pcap
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(n2tcap)

    file(GLOB n2tcap_files CONFIGURE_DEPENDS "n2tcap/**/*.cpp" "n2tcap/*.cpp")
    target_sources(n2tcap PRIVATE
        ${n2tcap_files}
        src/ip_address.cpp
        src/strerror.cpp
    )
endif()
