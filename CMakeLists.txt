cmake_minimum_required(VERSION 3.20)
project(AniLex VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt Automatic-Tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.10.1 REQUIRED COMPONENTS Core Gui Widgets Network Test Svg LinguistTools)
find_package(Qt6Keychain REQUIRED)

set(BUILD_WITH_QT6 ON  CACHE BOOL "" FORCE)
set(BUILD_TEST_APPLICATION OFF CACHE BOOL "" FORCE)
set(BUILD_TRANSLATIONS OFF CACHE BOOL "" FORCE)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

set(SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/generate_enums.py")
set(SCHEMA_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/api/operations/schema.graphql")
set(GENERATED_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/generated/anilist_enums.h")

add_custom_command(
        OUTPUT ${GENERATED_HEADER}
        COMMAND Python3::Interpreter ${SCRIPT}
        DEPENDS ${SCRIPT} ${SCHEMA_FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating C++ enums from GraphQL schema"
        VERBATIM
)

add_custom_target(generate_enums ALL DEPENDS ${GENERATED_HEADER})

include_directories(include)

add_subdirectory(src)
add_subdirectory(assets)
