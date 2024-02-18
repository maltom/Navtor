cmake_minimum_required(VERSION 3.21)

PROJECT(Navtor)


set(NAME CXXRescue)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable -Wno-unused-value -Wno-unused-local-typedefs -Wno-unused-private-field
")

set(RES_SOURCES
    Rescue.cpp
)

set(GEN_SOURCES
    LogGenerator.cpp
)

add_executable(${NAME} ${RES_SOURCES})
add_executable(FileGenerator ${GEN_SOURCES})
