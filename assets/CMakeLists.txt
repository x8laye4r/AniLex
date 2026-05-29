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
            images/ShiinaV2.jpg
            images/Shiina.jpg
            index.html
            designer_presets.jsonnet
            styles/main.qss
)