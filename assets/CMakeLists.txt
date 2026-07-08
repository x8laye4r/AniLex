qt_add_resources(${PROJECT_NAME} "anilex_assets"
        PREFIX "/"
        BASE "${CMAKE_SOURCE_DIR}/assets"
        FILES
            icons/Home.svg
            icons/Profile.svg
            icons/Browse.svg
            icons/Lists.svg
            icons/Search.svg
            CMakeLists.txt
            images/ShiinaV2.jpg
            images/Shiina.jpg
            index.html
            designer_presets.jsonnet
            styles/main.qss
)

qt_add_translations(${PROJECT_NAME} "anilex_translations"
        TS_FILES
            translations/anilex_en.ts
            translations/anilex_de.ts
)