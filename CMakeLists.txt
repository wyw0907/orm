cmake_minimum_required(VERSION 2.8)
project(wish.orm)

if (NOT OUTPUT_PATH)
set (OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/product/) 
endif()
message ("OUTPUT_PATH IS ${OUTPUT_PATH}")

if(CMAKE_COMPILER_IS_GNUCXX)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0)
        message(FATAL_ERROR "depend on c++17")
    endif()
else()
    message(FATAL_ERROR "only support for linux")
endif()

set (CMAKE_CXX_FLAGS "-std=c++17")

find_path(GTEST_INCLUDE_PATH gtest/gtest.h)
find_library(GTEST_LIB NAMES gtest gmock)
if ((NOT GTEST_INCLUDE_PATH) OR (NOT GTEST_LIB))
    message(FATAL_ERROR "fail to find gtest")
endif()

find_path(MYSQL_INCLUDE_PATH NAMES mysql/mysql.h)
find_library(MYSQL_LIB NAMES mysqlclient)
if ((NOT MYSQL_INCLUDE_PATH) OR (NOT MYSQL_LIB))
    message(FATAL_ERROR "fail to find mysql")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(test)
