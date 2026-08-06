add_library(anilex_sources SHARED
    main.cpp
)

target_include_directories(anilex_sources PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries(anilex_sources PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        Qt6::Svg
        Qt6Keychain::Qt6Keychain
        cppgraphqlgen::graphqlclient
        cppgraphqlgen::graphqlservice
)

qt_add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE anilex_sources)

qt_add_resources(${PROJECT_NAME} "graphql_queries"
        PREFIX "/graphql"
        BASE "${CMAKE_SOURCE_DIR}/src/api"
        FILES
)