cmake_minimum_required(VERSION 3.20)
project(AniLex VERSION 0.0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt Automatic-Tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets Network REQUIRED)

# Load all files
file(GLOB_RECURSE SOURCES "src/*.cpp")      # Gets all .cpp files
file(GLOB_RECURSE HEADERS "include/*.h")    # Gets all .h files
file(GLOB_RECURSE UI_FILES "ui/*.ui")       # Gets all .ui files
file(GLOB_RECURSE RESOURCE_LIST RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        CONFIGURE_DEPENDS
        "assets/*"
)                                           # Gets all resource files from assets as a relative file paths

configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/include/version.h.in"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/version.h"
        @ONLY
) # Generates version.h file

# Create a executable
qt_add_executable(
        ${PROJECT_NAME}
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
)

# Add the resources to the project
qt_add_resources(${PROJECT_NAME} "anilex_assets"
        PREFIX "/"
        FILES ${RESOURCE_LIST}
)


# link the includes dirs
target_include_directories(${PROJECT_NAME} PRIVATE include)

# link Qt to the project
target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
)