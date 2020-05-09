cmake_minimum_required (VERSION 3.15)
project (DawnCpp)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++2a")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

option(USE_CRONJOB "Use scheduling to run various cronjob" ON)
option(USE_RAPIDJSON "Use RapidJson for loading json data" ON)
option(USE_GFLAGS "Use gflags for argparse" OFF)
option(USE_LIBTELEGRAM "Use libtelegram as one of the telegram platform" ON)
option(USE_WEBSOCKET "Build websocket" ON)
option(BUILD_TEST "Build unittesting with gtest" ON)
option(USE_TWITTER "Use twitter API" ON)
option(USE_ABSEIL "Use abseil" ON)

set(3RDPARTY_INCLUDE_DIRS)
add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty)
set(COMPILE_DEFINITIONS "${COMPILE_DEFINITIONS};${3RDPARTY_COMPILE_DEFINITIONS}")

file(GLOB_RECURSE src_files
	"${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/library/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utility/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/external/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/trading/*.cpp"
)

# --- main binary ---
add_executable(DawnCpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp 
    ${src_files}
)

target_include_directories(
    DawnCpp
        PUBLIC
        ${3RDPARTY_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/src
)
target_compile_definitions(
    DawnCpp 
        PUBLIC 
        ${COMPILE_DEFINITIONS}
)
target_link_directories(
    DawnCpp
        PUBLIC
        ${CMAKE_BINARY_DIR}
)
target_link_libraries(
    DawnCpp
        ${3RDPARTY_LIBRARIES}
)

#[[

# ----- trading -----

add_executable(DawnTrade 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main_trading.cpp 
    ${src_files}
)

target_include_directories(
    DawnTrade
        PUBLIC
        ${3RDPARTY_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_compile_definitions(
    DawnTrade 
        PUBLIC 
        ${COMPILE_DEFINITIONS}
)
target_link_directories(
    DawnTrade
        PUBLIC
        ${CMAKE_BINARY_DIR}
)
target_link_libraries(
    DawnTrade
        ${3RDPARTY_LIBRARIES}
)


]]



# --- testing binary ---
file(GLOB_RECURSE test_files
	"${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp"
)

add_executable(unittest 
    ${test_files} 
    ${src_files}
)
target_include_directories(
    unittest
        PUBLIC
        ${3RDPARTY_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/src
)
target_compile_definitions(
    unittest 
        PUBLIC 
        ${COMPILE_DEFINITIONS}
)
target_link_directories(
    unittest
        PUBLIC
        ${CMAKE_BINARY_DIR}
)
target_link_libraries(
    unittest
        ${3RDPARTY_LIBRARIES}
)