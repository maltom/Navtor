cmake_minimum_required(VERSION 3.21)

PROJECT(Navtor)


set(NAME CXXRescue)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_COLOR_DIAGNOSTICS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable -Wno-unused-value -Wno-unused-local-typedefs")

set(RES_SOURCES
    FileFunctions.cpp
    main.cpp
)

set(RES_HEADERS
    FileFunctions.hpp
)

find_package(CryptoPP REQUIRED)

add_executable(${NAME} ${RES_SOURCES} ${RES_HEADERS})

target_link_libraries(${NAME} cryptopp-static)
