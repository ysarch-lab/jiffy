cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(confluo C CXX)

set(CONFLUO_VERSION_MAJOR "0")
set(CONFLUO_VERSION_MINOR "1")
set(CONFLUO_VERSION_PATCH "0")
set(CONFLUO_VERSION "${CONFLUO_VERSION_MAJOR}.${CONFLUO_VERSION_MINOR}.${CONFLUO_VERSION_PATCH}")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  message(FATAL_ERROR "Windows build not supported")
endif()

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-modules") 
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(SUPPRESS_WARN_FLAGS 
    "${SUPPRESS_WARN_FLAGS} -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-result -Wno-conversion-null" )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(SUPPRESS_WARN_FLAGS 
    "${SUPPRESS_WARN_FLAGS} -Wno-unused-function -Wno-unused-variable -Wno-unused-parameter -Wno-missing-field-initializers -Wno-gnu-zero-variadic-macro-arguments -Wno-unused-private-field -Wno-unused-const-variable")
endif()

# SUPPRESS_WARN_FLAGS must go **after** all flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall -pedantic -Wextra ${SUPPRESS_WARN_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pedantic -Wextra ${SUPPRESS_WARN_FLAGS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -Wall -pedantic -Wextra ${SUPPRESS_WARN_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ldl")

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "" FORCE)
endif()

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed.")
endif ()

set(CMAKE_VERBOSE_MAKEFILE false
    CACHE STRING "Show all outputs including compiler lines." FORCE)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/cmake")

if(APPLE)
  # disable warnings about "ranlib: file: xxx.a(...cc.o) has no symbols"
  set(CMAKE_C_ARCHIVE_FINISH   "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
  set(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
endif()

set(EXTERNAL_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ${CMAKE_CXX_FLAGS_${UPPERCASE_BUILD_TYPE}}")
set(EXTERNAL_C_FLAGS "${CMAKE_C_FLAGS} -fPIC ${CMAKE_C_FLAGS_${UPPERCASE_BUILD_TYPE}}")

include (ExternalProject)

ExternalProject_Add(lz4
        URL https://github.com/lz4/lz4/archive/v1.8.2.tar.gz
        CONFIGURE_COMMAND ""
        BUILD_IN_SOURCE 1
        BUILD_COMMAND make -C lib lib MOREFLAGS=-fPIC
        INSTALL_COMMAND ""
)
ExternalProject_Get_Property(lz4 SOURCE_DIR BINARY_DIR)
set(lz4_INCLUDE_DIR "${SOURCE_DIR}/lib")
set(lz4_STATIC_LIB "${BINARY_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}lz4${CMAKE_STATIC_LIBRARY_SUFFIX}")
include_directories(SYSTEM ${lz4_INCLUDE_DIR})

include_directories(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/libconfluo
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/aggregate
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/conf
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/bitmap
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/cursor
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/lazy
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/monolog
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/sketch
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/schema
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/storage
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/types
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/utils
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/archival
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/archival/io
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/compression
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/parser
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/planner
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/threads
        ${CMAKE_SOURCE_DIR}/libutils/utils
)

add_executable(schema main.cc)
target_link_libraries(schema confluo confluoutils)

# Options
#include(BuildOptions)

# Functions
#include(Functions)

# Dependencies
#include(Dependencies)

# Utilities
add_subdirectory(libutils)

# Core library
add_subdirectory(libconfluo)