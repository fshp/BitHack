cmake_minimum_required(VERSION 2.8)
project(BitHack)

find_package(OpenSSL REQUIRED)
if(NOT OPENSSL_FOUND)
    message(SEND_ERROR "Failed to find OpenSSL")
    return()
else()
    include_directories(${OPENSSL_INCLUDE_DIR})
endif()

find_package(Boost REQUIRED)
if(NOT Boost_FOUND)
    message(SEND_ERROR "Failed to find Boost Format")
    return()
else()
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -s -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -s -O3")

set(SOURCE_EXE src/main.cpp
				src/CKeyFactory.cpp
				src/CKey.cpp
				src/base58.c
				src/utils.c)
add_executable(BitHack ${SOURCE_EXE})

target_link_libraries(BitHack ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} pthread)
