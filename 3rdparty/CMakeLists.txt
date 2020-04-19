cmake_minimum_required (VERSION 3.11)

set(CRONCPP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/croncpp)
set(RAPIDJSON_SRC ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson)
set(FMT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/fmt)
set(DELEGATE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/delegate)
set(LIBTELEGRAM_SRC ${CMAKE_CURRENT_SOURCE_DIR}/libtelegram)
set(TWITCURL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/twitcurl)
set(SPDLOG_SRC ${CMAKE_CURRENT_SOURCE_DIR}/spdlog)
set(LIBWEBSOCKETS_CMAKE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindLibWebsockets.cmake)
set(ABSEIL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/abseil)


set(3RDPARTY_INCLUDE_DIRS)
set(3RDPARTY_LIBRARIES)
set(3RDPARTY_COMPILE_DEFINITIONS)

# Croncpp
if(USE_CRONJOB STREQUAL "ON")
    message(STATUS "Cron source dir ${CRONCPP_SRC}")
    list(APPEND 3RDPARTY_INCLUDE_DIRS "${CRONCPP_SRC}/include")
    set(3RDPARTY_COMPILE_DEFINITIONS "${3RDPARTY_COMPILE_DEFINITIONS};USE_CRON")
endif()

# Gtest
if(BUILD_TEST STREQUAL "ON")
    enable_testing()
    find_package(GTest REQUIRED)
    list(APPEND 3RDPARTY_INCLUDE_DIRS "${GTEST_INCLUDE_DIRS}")
    list(APPEND 3RDPARTY_LIBRARIES "${GTEST_BOTH_LIBRARIES}")
endif()

# Rapidjson
if (USE_RAPIDJSON STREQUAL "ON")
    message(STATUS "RapidJson source dir ${RAPIDJSON_SRC}")
    list(APPEND 3RDPARTY_INCLUDE_DIRS "${RAPIDJSON_SRC}/include")
    set(3RDPARTY_COMPILE_DEFINITIONS "${3RDPARTY_COMPILE_DEFINITIONS};USE_RAPIDJSON")
endif()

# Fmt
message(STATUS "Fmt source dir ${FMT_SRC}")
add_subdirectory(${FMT_SRC})
list(APPEND 3RDPARTY_INCLUDE_DIRS "${FMT_SRC}/include")
list(APPEND 3RDPARTY_LIBRARIES fmt)

# Spdlog
message(STATUS "Spdlog source dir ${SPDLOG_SRC}")
add_subdirectory(${SPDLOG_SRC})
list(APPEND 3RDPARTY_INCLUDE_DIRS "${SPDLOG_SRC}/include")
list(APPEND 3RDPARTY_LIBRARIES spdlog)

# Delegate
message(STATUS "Delegate source dir ${DELEGATE_SRC}")
list(APPEND 3RDPARTY_INCLUDE_DIRS "${DELEGATE_SRC}/include")

# curl
if (APPLE)
    # Use brew updated curl instead of system curl
    set(CURL_ROOT_DIR /usr/local/opt/curl/)
    set(CURL_INCLUDE_DIRS "${CURL_ROOT_DIR}/include")
    message(STATUS "Curl source dir ${CURL_ROOT_DIR}")
    list(APPEND 3RDPARTY_INCLUDE_DIRS "${CURL_ROOT_DIR}/include")
    list(APPEND 3RDPARTY_LIBRARIES curl)
    configure_file(${CURL_ROOT_DIR}/lib/libcurl.dylib ${CMAKE_BINARY_DIR}/libcurl.dylib COPYONLY)
    set(CURL_FOUND TRUE)
else()
    find_package(CURL REQUIRED)
endif()
list(APPEND 3RDPARTY_LIBRARIES "${CURL_LIBRARIES}")

if (USE_LIBTELEGRAM STREQUAL "ON")
    if(APPLE)
        set(OPENSSL_ROOT_DIR /usr/local/opt/openssl)
        set(OPENSSL_LIBRARIES /usr/local/opt/openssl/lib)
    elseif(LINUX) 
        set(OPENSSL_ROOT_DIR /usr/local/ssl)
        set(OPENSSL_LIBRARIES /usr/local/ssl/lib)
    endif()
    find_package(OpenSSL)
endif()
set(OPENSSL_INCLUDE_DIRS "${OPENSSL_ROOT_DIR}/include")

#libtelegram
if(USE_LIBTELEGRAM STREQUAL "ON")
    message(STATUS "Libtelegram source dir ${LIBTELEGRAM_SRC}")
    list(APPEND 3RDPARTY_INCLUDE_DIRS "${LIBTELEGRAM_SRC}/include")
    add_subdirectory(${LIBTELEGRAM_SRC})
    set(3RDPARTY_COMPILE_DEFINITIONS "${3RDPARTY_COMPILE_DEFINITIONS};USE_LIBTELEGRAM")
    list(APPEND 3RDPARTY_LIBRARIES libtelegram)
endif()

# Twitter
if(USE_TWITTER STREQUAL "ON")
    message(STATUS "Twitcurl source dir ${TWITCURL_SRC}")
    list(APPEND 3RDPARTY_INCLUDE_DIRS "${TWITCURL_SRC}/include")
    add_subdirectory(${TWITCURL_SRC})
    set(3RDPARTY_COMPILE_DEFINITIONS "${3RDPARTY_COMPILE_DEFINITIONS};USE_TWITTER")
    list(APPEND 3RDPARTY_LIBRARIES twitcurl)
endif()

# websocket
if(USE_WEBSOCKET STREQUAL "ON")
    find_package(Libwebsockets)
    list(APPEND 3RDPARTY_INCLUDE_DIRS "${LIBWEBSOCKETS_INCLUDE_DIR}")
    list(APPEND 3RDPARTY_LIBRARIES websockets)
    if(APPLE)
        configure_file(/usr/local/lib/libwebsockets.dylib ${CMAKE_BINARY_DIR}/libwebsockets.dylib COPYONLY)
    elseif(LINUX) 
        configure_file(/usr/lib/libwebsockets.so ${CMAKE_BINARY_DIR}/libwebsockets.so COPYONLY)
    endif()  
endif()

# abseil
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
if(USE_ABSEIL STREQUAL "ON")
    message(STATUS "Abseil source dir ${ABSEIL_SRC}")
    add_subdirectory(${ABSEIL_SRC})
    list(APPEND 3RDPARTY_INCLUDE_DIRS "${ABSEIL_SRC}")
    list(APPEND 3RDPARTY_LIBRARIES absl::base absl::synchronization absl::strings absl::flags absl::flags_parse
        absl::flat_hash_map)
    set(3RDPARTY_COMPILE_DEFINITIONS "${3RDPARTY_COMPILE_DEFINITIONS};USE_ABSEIL")
endif()


set(3RDPARTY_INCLUDE_DIRS ${3RDPARTY_INCLUDE_DIRS} PARENT_SCOPE)
set(3RDPARTY_COMPILE_DEFINITIONS ${3RDPARTY_COMPILE_DEFINITIONS} PARENT_SCOPE)
set(3RDPARTY_LIBRARIES ${3RDPARTY_LIBRARIES} PARENT_SCOPE)