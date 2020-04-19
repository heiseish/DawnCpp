cmake_minimum_required(VERSION 2.8.4)
project(twitcurl)

find_package(PkgConfig)
# find_package(CURL 7.56.0)
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
endif()

set(twitSrcs src/base64.cpp src/HMAC_SHA1.cpp src/oauthlib.cpp src/SHA1.cpp src/urlencode.cpp src/twitcurl.cpp)

add_library(${PROJECT_NAME} ${twitSrcs})
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        ${PKGS_INCLUDE_DIRS} 
        include/twitcurl
)
