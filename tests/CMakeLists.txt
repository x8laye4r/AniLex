qt_add_executable(${PROJECT_NAME}_TEST TestSecret.cpp)

target_include_directories(${PROJECT_NAME}_TEST PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(${PROJECT_NAME}_TEST PRIVATE
        anilex_lib
        Qt6::Test
)

add_test(NAME AniLexTests COMMAND ${PROJECT_NAME}_TEST)