cmake_minimum_required(VERSION 3.20)
project(AniLex VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt Automatic-Tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.10.1 REQUIRED COMPONENTS Core Gui Widgets Network Test Svg LinguistTools)
find_package(Qt6Keychain REQUIRED)
find_package(cppgraphqlgen CONFIG REQUIRED)

set(BUILD_WITH_QT6 ON  CACHE BOOL "" FORCE)
set(BUILD_TEST_APPLICATION OFF CACHE BOOL "" FORCE)
set(BUILD_TRANSLATIONS OFF CACHE BOOL "" FORCE)

add_subdirectory(src)
add_subdirectory(assets)
