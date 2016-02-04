cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME MCTS_Test)
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

set(SOURCES
    src/main.cpp
    src/GameXO.cpp
    src/GameXO_4x4x4.cpp
    src/PlayerAB.cpp
    src/PlayerMinimax.cpp
    src/PlayerMCTS.cpp
    src/Tournament.cpp
    src/Utils.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} pthread)
