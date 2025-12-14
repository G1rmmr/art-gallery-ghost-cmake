cmake_minimum_required(VERSION 3.28)
project(art-gallery-ghost LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_BUNDLE "Build as macOS app bundle" OFF)

set(SOURCES main.cpp)
set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")

set(SFML_VERSION "3.0.2")

find_package(SFML 3 COMPONENTS system window graphics audio network QUIET)

if(NOT SFML_FOUND)
    message(STATUS "SFML not found. Using FetchContent.")
    set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS ON CACHE BOOL "Suppress dev warnings" FORCE)
    set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "Suppress deprecation warnings" FORCE)

    if(POLICY CMP0153)
        cmake_policy(SET CMP0153 OLD)
    endif()

    set(SFML_REPOSITORY https://github.com/SFML/SFML.git)
    set(SFML_TAG ${SFML_VERSION})

    include(FetchContent)
    FetchContent_Declare(SFML
        GIT_REPOSITORY ${SFML_REPOSITORY}
        GIT_TAG ${SFML_TAG}
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
    FetchContent_MakeAvailable(SFML)
endif()

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
    sfml-network
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(art-gallery-ghost PRIVATE DEBUG_MODE)
    message(STATUS "Debug mode enabled")
endif()

if(APPLE)
    target_compile_definitions(art-gallery-ghost PRIVATE GL_SILENCE_DEPRECATION)
endif()

target_compile_definitions(
    art-gallery-ghost
    PRIVATE
    "ASSET_DIR=\"${ASSETS_DIR}\""
)

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
        COMMAND codesign --force --deep --sign - $<TARGET_BUNDLE_DIR:art-gallery-ghost>
        COMMENT "Code signing the app bundle"
    )
endif()

