cmake_minimum_required(VERSION 2.8)
project(BitHack)

find_package(OpenSSL REQUIRED)
if(NOT OPENSSL_FOUND)
    message(SEND_ERROR "Failed to find OpenSSL")
    return()
else()
    include_directories(${OPENSSL_INCLUDE_DIR})
endif()

set(SOURCE_EXE src/main.c)
add_executable(BitHack ${SOURCE_EXE})

target_link_libraries(BitHack ${OPENSSL_LIBRARIES})
