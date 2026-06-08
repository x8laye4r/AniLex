cmake_minimum_required(VERSION 3.20)
project(AniLex VERSION 0.0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt Automatic-Tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC ON)

include(FetchContent)

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

# ================
# Set all files
# ================
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/*.h")
file(GLOB_RECURSE UI_FILES CONFIGURE_DEPENDS "ui/*.ui")

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "tests/*.cpp")
file(GLOB_RECURSE TEST_HEADERS CONFIGURE_DEPENDS "tests/*.h")

file(GLOB_RECURSE RESOURCE_LIST RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        CONFIGURE_DEPENDS
        "assets/*"
)

file(GLOB_RECURSE GRAPHQL_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/api/*.graphql"
)

file(GLOB_RECURSE OLD_GRAPHQL_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/api/old_queries/*.graphql"
)

list(REMOVE_ITEM GRAPHQL_FILES ${OLD_GRAPHQL_FILES})


configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/include/version.h.in"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/version.h"
        @ONLY
)


# ================
# Build app code as a library so tests can link it
# ================
add_library(anilex_lib SHARED
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
)

target_include_directories(anilex_lib PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(anilex_lib PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        Qt6::Svg
)

target_link_libraries(anilex_lib PUBLIC Qt6Keychain::Qt6Keychain)

# ================
# Main executable
# ================
qt_add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE anilex_lib)

qt_add_resources(${PROJECT_NAME} "anilex_assets"
        PREFIX "/"
        FILES ${RESOURCE_LIST}
)
qt_add_resources(${PROJECT_NAME} "graphql_queries"
        PREFIX "/graphql"
        BASE "${CMAKE_CURRENT_SOURCE_DIR}/src/api"
        FILES ${GRAPHQL_FILES}
)

qt_add_translations(${PROJECT_NAME} "anilex_translations"
        TS_FILES
        assets/translations/anilex_en_US.ts
            assets/translations/anilex_de.ts
)

# ================
# Tests
# ================
enable_testing()

qt_add_executable(${PROJECT_NAME}_TEST
        ${TEST_SOURCES}
        ${TEST_HEADERS}
)

target_include_directories(${PROJECT_NAME}_TEST PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests"
)

target_link_libraries(${PROJECT_NAME}_TEST PRIVATE
        anilex_lib
        Qt6::Test
)

# Don't know if I need them
qt_add_resources(${PROJECT_NAME}_TEST "anilex_assets"
        PREFIX "/"
        FILES ${RESOURCE_LIST}
)

add_test(NAME AniLexTests COMMAND ${PROJECT_NAME}_TEST)