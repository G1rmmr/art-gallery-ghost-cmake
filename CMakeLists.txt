cmake_minimum_required(VERSION 3.28)
project(art-gallery-ghost LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_BUNDLE "Build as macOS app bundle" OFF)

find_package(SFML 2.6 COMPONENTS system window graphics audio)
if(NOT SFML_FOUND)
    message(STATUS "SFML not found via find_package. Using FetchContent.")
    include(FetchContent)
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
    FetchContent_MakeAvailable(SFML)
endif()

set(SOURCES main.cpp)
set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")

if(BUILD_BUNDLE AND APPLE)
    add_executable(art-gallery-ghost MACOSX_BUNDLE ${SOURCES})
else()
    add_executable(art-gallery-ghost ${SOURCES})
endif()

target_compile_features(art-gallery-ghost PRIVATE cxx_std_20)
target_include_directories(
    art-gallery-ghost
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/engine
)
target_link_libraries(art-gallery-ghost PRIVATE
    sfml-graphics
    sfml-system
    sfml-window
    sfml-audio
)

option(ENABLE_PROFILE "Enable performance profiling" OFF)

if(ENABLE_PROFILE)
    target_compile_definitions(
        art-gallery-ghost
        PRIVATE
        ENABLE_PROFILE=1
        "ASSET_DIR=\"${ASSETS_DIR}\""
    )
    message(STATUS "Profiling enabled")
else()
    target_compile_definitions(
        art-gallery-ghost
        PRIVATE
        ENABLE_PROFILE=0
        "ASSET_DIR=\"${ASSETS_DIR}\"")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(
        art-gallery-ghost
        PRIVATE
        DEBUG_MODE
        "ASSET_DIR=\"${ASSETS_DIR}\""
    )
    message(STATUS "Debug mode enabled - console logging active")
endif()

if(APPLE)
    target_compile_definitions(
        art-gallery-ghost
        PRIVATE
        GL_SILENCE_DEPRECATION
        "ASSET_DIR=\"${ASSETS_DIR}\""
    )
endif()

if(BUILD_BUNDLE AND APPLE)
    set_target_properties(art-gallery-ghost PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "Art Gallery Ghost"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.example.art-gallery-ghost"
        MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
        MACOSX_BUNDLE_COPYRIGHT "© 2025 John Nam"
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
    )

    add_custom_command(TARGET art-gallery-ghost POST_BUILD
        COMMAND codesign
            --force
            --deep
            --sign -
            $<TARGET_BUNDLE_DIR:art-gallery-ghost>
        COMMENT "Code signing the app bundle"
    )
endif()
