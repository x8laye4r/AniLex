cmake_minimum_required(VERSION 3.20)
project(AniLex VERSION 0.0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt Automatic-Tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.10.1 REQUIRED COMPONENTS Core Gui Widgets Network Test Svg LinguistTools)
find_package(Qt6Keychain REQUIRED)

if(Qt6Core_VERSION VERSION_LESS "6.10.1")
    message(WARNING
            "This project was developed and tested with Qt 6.10.1. "
            "Current Qt version: ${Qt6Core_VERSION}"
    )
endif()

set(BUILD_WITH_QT6 ON  CACHE BOOL "" FORCE)
set(BUILD_TEST_APPLICATION OFF CACHE BOOL "" FORCE)
set(BUILD_TRANSLATIONS OFF CACHE BOOL "" FORCE)

configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/include/version.h.in"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/version.h"
        @ONLY
)

add_subdirectory(src)
add_subdirectory(assets)

if (BUILD_TEST_APPLICATION)
    enable_testing()
    add_subdirectory(tests)
endif()