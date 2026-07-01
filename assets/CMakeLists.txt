qt_add_resources(${PROJECT_NAME} "anilex_assets"
        PREFIX "/"
        BASE "${CMAKE_SOURCE_DIR}/assets"
        FILES
            icons/Home.svg
            icons/Profile.svg
            icons/Browse.svg
            icons/Lists.svg
            icons/Search.svg
            script.js
            styles.css
            CMakeLists.txt
            index.html
            designer_presets.jsonnet
            designer_presets.json
            styles/main.qss
)

qt_add_translations(${PROJECT_NAME} "anilex_translations"
        TS_FILES
            translations/anilex_en.ts
            translations/anilex_de.ts
)