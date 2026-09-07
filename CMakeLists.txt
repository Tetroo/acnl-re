cmake_minimum_required(VERSION 3.20)
project(acnl_re LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/types
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Core library sources
set(ACNL_CORE_SOURCES
    src/core/RomfsLoader.cpp
    src/world/TownGenerator.cpp
    src/time/TimeManager.cpp
)

add_library(acnl_core STATIC ${ACNL_CORE_SOURCES})

# Executable test harness
add_executable(acnl_town_gen src/main.cpp)
target_link_libraries(acnl_town_gen PRIVATE acnl_core)
