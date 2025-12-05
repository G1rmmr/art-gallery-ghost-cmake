cmake_minimum_required(VERSION 3.28)
project(art-gallery-ghost LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 개발/릴리즈 모드 구분
option(BUILD_BUNDLE "Build as macOS app bundle" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)

FetchContent_MakeAvailable(SFML)

# 소스 파일
set(SOURCES main.cpp)

set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")

# 개발 모드: 일반 실행파일, 릴리즈: 번들
if(BUILD_BUNDLE AND APPLE)
    add_executable(art-gallery-ghost MACOSX_BUNDLE ${SOURCES})
else()
    add_executable(art-gallery-ghost ${SOURCES})
endif()

target_compile_features(art-gallery-ghost PRIVATE cxx_std_20)
target_include_directories(
    art-gallery-ghost
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(art-gallery-ghost PRIVATE
    SFML::Graphics
    SFML::System
    SFML::Window
    SFML::Audio
)

# 프로파일 옵션 추가
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

# 빌드 모드 정의
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(
        art-gallery-ghost
        PRIVATE
        DEBUG_MODE
        "ASSET_DIR=\"${ASSETS_DIR}\""
    )
    message(STATUS "Debug mode enabled - console logging active")
endif()

# macOS 설정
if(APPLE)
    target_compile_definitions(
        art-gallery-ghost
        PRIVATE
        GL_SILENCE_DEPRECATION
        "ASSET_DIR=\"${ASSETS_DIR}\""
    )
endif()

# macOS App Bundle 설정 (BUILD_BUNDLE이 ON일 때만)
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

    # 코드 사이닝 설정 (개발용)
    add_custom_command(TARGET art-gallery-ghost POST_BUILD
        COMMAND codesign
            --force
            --deep
            --sign -
            $<TARGET_BUNDLE_DIR:art-gallery-ghost>
        COMMENT "Code signing the app bundle"
    )
endif()
