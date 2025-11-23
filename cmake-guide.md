# CMAKE를 활용한 멀티플랫폼 C++ 개발자가 알아야할 것들

## 목차
1. [CMake 기본 구조](#1-cmake-기본-구조)
2. [플랫폼별 처리](#2-플랫폼별-처리)
3. [컴파일러 설정](#3-컴파일러-설정)
4. [외부 라이브러리 관리](#4-외부-라이브러리-관리)
5. [프로젝트 구조화](#5-프로젝트-구조화)
6. [설치 및 패키징](#6-설치-및-패키징)
7. [테스트](#7-테스트)
8. [빌드 타입별 설정](#8-빌드-타입별-설정)
9. [옵션 및 캐시 변수](#9-옵션-및-캐시-변수)
10. [고급 기능](#10-고급-기능)
11. [보안 설정](#11-보안-설정)
12. [GitHub Actions CI/CD](#12-github-actions-cicd)
13. [실전 예제](#13-실전-예제---크로스-플랫폼-게임)
14. [유용한 CMake 명령어](#14-유용한-cmake-명령어)

---

## 1. CMake 기본 구조

### 최소 CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.20)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(myapp main.cpp)
```

### 중요한 변수들
```cmake
# 프로젝트 정보
PROJECT_NAME                # 프로젝트 이름
PROJECT_VERSION            # 버전
PROJECT_SOURCE_DIR         # 소스 디렉토리
PROJECT_BINARY_DIR         # 빌드 디렉토리

# 빌드 설정
CMAKE_BUILD_TYPE           # Debug, Release, RelWithDebInfo, MinSizeRel
CMAKE_CXX_COMPILER         # C++ 컴파일러 경로
CMAKE_SYSTEM_NAME          # 플랫폼 (Windows, Linux, Darwin)
CMAKE_SYSTEM_PROCESSOR     # CPU 아키텍처 (x86_64, arm64)

# 출력 디렉토리
CMAKE_RUNTIME_OUTPUT_DIRECTORY  # 실행파일 출력 위치
CMAKE_LIBRARY_OUTPUT_DIRECTORY  # 공유 라이브러리 출력 위치
CMAKE_ARCHIVE_OUTPUT_DIRECTORY  # 정적 라이브러리 출력 위치
```

---

## 2. 플랫폼별 처리

### 플랫폼 감지
```cmake
if(WIN32)
    # Windows
    message(STATUS "Building for Windows")
elseif(APPLE)
    # macOS
    message(STATUS "Building for macOS")
elseif(UNIX AND NOT APPLE)
    # Linux
    message(STATUS "Building for Linux")
endif()

# 더 세밀한 검사
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Windows 전용
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # macOS 전용
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Linux 전용
endif()
```

### 아키텍처 감지
```cmake
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    message(STATUS "x86_64 architecture")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64|ARM64")
    message(STATUS "ARM64 architecture")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm|ARM")
    message(STATUS "ARM architecture")
endif()
```

### 플랫폼별 소스 파일
```cmake
set(COMMON_SOURCES
    src/main.cpp
    src/core.cpp
)

if(WIN32)
    list(APPEND COMMON_SOURCES src/platform/windows.cpp)
elseif(APPLE)
    list(APPEND COMMON_SOURCES src/platform/macos.mm)
elseif(UNIX)
    list(APPEND COMMON_SOURCES src/platform/linux.cpp)
endif()

add_executable(myapp ${COMMON_SOURCES})
```

---

## 3. 컴파일러 설정

### 컴파일러별 플래그
```cmake
if(MSVC)
    # Visual Studio
    target_compile_options(myapp PRIVATE
        /W4           # 경고 레벨 4
        /WX           # 경고를 에러로
        /permissive-  # 표준 준수 모드
        /utf-8        # UTF-8 소스 파일
    )
    
    # Debug/Release 설정
    target_compile_options(myapp PRIVATE
        $<$<CONFIG:Debug>:/Od /Zi>           # 최적화 없음, 디버그 정보
        $<$<CONFIG:Release>:/O2 /DNDEBUG>    # 최적화, NDEBUG 정의
    )
else()
    # GCC/Clang
    target_compile_options(myapp PRIVATE
        -Wall -Wextra -Wpedantic
        -Werror
    )
    
    # Debug/Release 설정
    target_compile_options(myapp PRIVATE
        $<$<CONFIG:Debug>:-O0 -g>
        $<$<CONFIG:Release>:-O3 -DNDEBUG>
    )
    
    # Clang 전용
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(myapp PRIVATE
            -Wno-missing-field-initializers
        )
    endif()
endif()
```

### 컴파일러 기능 확인
```cmake
include(CheckCXXCompilerFlag)

check_cxx_compiler_flag("-std=c++20" COMPILER_SUPPORTS_CXX20)
if(COMPILER_SUPPORTS_CXX20)
    set(CMAKE_CXX_STANDARD 20)
else()
    message(FATAL_ERROR "Compiler doesn't support C++20")
endif()
```

---

## 4. 외부 라이브러리 관리

### find_package (시스템 라이브러리)
```cmake
# 기본 사용
find_package(OpenCV REQUIRED)
target_link_libraries(myapp PRIVATE OpenCV::OpenCV)

# 버전 지정
find_package(Boost 1.75 REQUIRED COMPONENTS system filesystem)
target_link_libraries(myapp PRIVATE Boost::system Boost::filesystem)

# 옵션으로 찾기
find_package(Qt6 COMPONENTS Widgets)
if(Qt6_FOUND)
    target_link_libraries(myapp PRIVATE Qt6::Widgets)
endif()
```

### FetchContent (소스 다운로드)
```cmake
include(FetchContent)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.1.1
    GIT_SHALLOW ON
)

FetchContent_MakeAvailable(fmt)
target_link_libraries(myapp PRIVATE fmt::fmt)

# 여러 라이브러리
FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
)

FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)

FetchContent_MakeAvailable(spdlog json)
target_link_libraries(myapp PRIVATE 
    spdlog::spdlog
    nlohmann_json::nlohmann_json
)
```

### ExternalProject (복잡한 빌드)
```cmake
include(ExternalProject)

ExternalProject_Add(
    external_lib
    URL https://example.com/lib.tar.gz
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/external
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
)
```

---

## 5. 프로젝트 구조화

### 서브디렉토리
```cmake
# 루트 CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(MyProject)

add_subdirectory(src)
add_subdirectory(libs)
add_subdirectory(tests)

# src/CMakeLists.txt
add_executable(myapp 
    main.cpp
    core.cpp
)

# libs/CMakeLists.txt
add_library(mylib STATIC
    mylib.cpp
)
```

### 라이브러리 생성
```cmake
# 정적 라이브러리
add_library(mylib_static STATIC
    src/lib.cpp
)

# 동적 라이브러리
add_library(mylib_shared SHARED
    src/lib.cpp
)

# 헤더 온리 라이브러리
add_library(mylib_interface INTERFACE)
target_include_directories(mylib_interface INTERFACE include/)

# 공개 헤더 설정
target_include_directories(mylib_static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
```

---

## 6. 설치 및 패키징

### 설치 규칙
```cmake
# 실행파일 설치
install(TARGETS myapp
    RUNTIME DESTINATION bin
)

# 라이브러리 설치
install(TARGETS mylib
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# 헤더 파일 설치
install(DIRECTORY include/
    DESTINATION include
)

# 설정 파일 설치
install(FILES config/settings.json
    DESTINATION share/myapp
)
```

### CPack (패키징)
```cmake
include(CPack)

set(CPACK_PACKAGE_NAME "MyApp")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VENDOR "Your Company")

if(WIN32)
    set(CPACK_GENERATOR "NSIS;ZIP")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
elseif(UNIX)
    set(CPACK_GENERATOR "DEB;RPM;TGZ")
endif()
```

---

## 7. 테스트

### CTest
```cmake
enable_testing()

add_executable(test_core tests/test_core.cpp)
target_link_libraries(test_core PRIVATE mylib)

add_test(NAME CoreTest COMMAND test_core)

# Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

add_executable(gtest_example tests/gtest_example.cpp)
target_link_libraries(gtest_example PRIVATE 
    GTest::gtest_main
    mylib
)

include(GoogleTest)
gtest_discover_tests(gtest_example)
```

---

## 8. 빌드 타입별 설정

```cmake
# 커스텀 빌드 타입
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel" CACHE STRING "" FORCE)

# 빌드 타입별 정의
target_compile_definitions(myapp PRIVATE
    $<$<CONFIG:Debug>:DEBUG_MODE>
    $<$<CONFIG:Release>:NDEBUG RELEASE_MODE>
)

# 빌드 타입별 플래그
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
```

---

## 9. 옵션 및 캐시 변수

```cmake
# 옵션
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(ENABLE_TESTS "Enable testing" ON)
option(USE_SYSTEM_LIBS "Use system libraries" OFF)

if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# 캐시 변수
set(MAX_THREADS "4" CACHE STRING "Maximum number of threads")
set(ENABLE_LOGGING ON CACHE BOOL "Enable logging")

# 사용자 입력 검증
if(MAX_THREADS LESS 1)
    message(FATAL_ERROR "MAX_THREADS must be at least 1")
endif()
```

---

## 10. 고급 기능

### Generator Expressions
```cmake
# 설정별 값
target_compile_definitions(myapp PRIVATE
    $<$<CONFIG:Debug>:DEBUG_BUILD>
    $<$<CONFIG:Release>:RELEASE_BUILD>
)

# 플랫폼별 값
target_sources(myapp PRIVATE
    $<$<PLATFORM_ID:Windows>:src/windows.cpp>
    $<$<PLATFORM_ID:Darwin>:src/macos.cpp>
    $<$<PLATFORM_ID:Linux>:src/linux.cpp>
)

# 컴파일러별 플래그
target_compile_options(myapp PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:GNU>:-Wall>
    $<$<CXX_COMPILER_ID:Clang>:-Weverything>
)
```

### 사전 컴파일 헤더 (PCH)
```cmake
target_precompile_headers(myapp PRIVATE
    <vector>
    <string>
    <memory>
    <iostream>
)
```

### Unity Build (점보 빌드)
```cmake
set_target_properties(myapp PROPERTIES
    UNITY_BUILD ON
    UNITY_BUILD_BATCH_SIZE 16
)
```

---

## 11. 보안 설정

### 컴파일러 보안 플래그

#### GCC/Clang (Linux/macOS)
```cmake
if(NOT MSVC)
    target_compile_options(myapp PRIVATE
        # Stack protection
        -fstack-protector-strong
        
        # Position Independent Executable
        -fPIE
        
        # Format string attack protection
        -Wformat -Wformat-security
        
        # Buffer overflow detection
        -D_FORTIFY_SOURCE=2
        
        # Stack canaries
        -fstack-clash-protection
    )
    
    target_link_options(myapp PRIVATE
        # Full RELRO (RELocation Read-Only)
        -Wl,-z,relro,-z,now
        
        # Position Independent Executable
        -pie
        
        # No executable stack
        -Wl,-z,noexecstack
    )
endif()
```

#### MSVC (Windows)
```cmake
if(MSVC)
    target_compile_options(myapp PRIVATE
        # Control Flow Guard
        /guard:cf
        
        # Buffer Security Check
        /GS
        
        # SDL checks
        /sdl
    )
    
    target_link_options(myapp PRIVATE
        # Control Flow Guard
        /GUARD:CF
        
        # Data Execution Prevention
        /NXCOMPAT
        
        # Address Space Layout Randomization
        /DYNAMICBASE
        
        # Safe SEH
        /SAFESEH
    )
endif()
```

### 종합 보안 설정 함수
```cmake
function(enable_security_features target)
    if(MSVC)
        target_compile_options(${target} PRIVATE
            /guard:cf
            /GS
            /sdl
        )
        target_link_options(${target} PRIVATE
            /GUARD:CF
            /NXCOMPAT
            /DYNAMICBASE
        )
    else()
        target_compile_options(${target} PRIVATE
            -fstack-protector-strong
            -fPIE
            -Wformat -Wformat-security
            -D_FORTIFY_SOURCE=2
        )
        
        if(NOT APPLE)
            target_compile_options(${target} PRIVATE
                -fstack-clash-protection
            )
        endif()
        
        target_link_options(${target} PRIVATE
            -Wl,-z,relro,-z,now
            -pie
            -Wl,-z,noexecstack
        )
    endif()
    
    # Disable RTTI if not needed (reduces attack surface)
    # target_compile_options(${target} PRIVATE
    #     $<$<CXX_COMPILER_ID:MSVC>:/GR->
    #     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fno-rtti>
    # )
endfunction()

# 사용
add_executable(myapp main.cpp)
enable_security_features(myapp)
```

### 민감한 정보 관리

#### 환경 변수 사용
```cmake
# API 키나 비밀번호를 환경 변수로 받기
set(API_KEY $ENV{MY_API_KEY})

if(NOT API_KEY)
    message(WARNING "API_KEY not set in environment")
    set(API_KEY "development_key")
endif()

target_compile_definitions(myapp PRIVATE
    API_KEY="${API_KEY}"
)
```

#### 설정 파일 템플릿
```cmake
# config.hpp.in 파일 생성
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.hpp
    @ONLY
)

target_include_directories(myapp PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)
```

`config.hpp.in`:
```cpp
#pragma once

// API 키는 환경 변수나 빌드 시 주입
#define API_KEY "@API_KEY@"
#define VERSION "@PROJECT_VERSION@"

// 민감한 정보는 절대 소스 코드에 직접 넣지 말 것!
```

`.gitignore`에 추가:
```
# 빌드 결과물
build*/
*.user

# 민감한 설정 파일
config.hpp
secrets.cmake
.env
```

### 의존성 보안

#### 체크섬 검증 (FetchContent)
```cmake
include(FetchContent)

FetchContent_Declare(
    fmt
    URL https://github.com/fmtlib/fmt/archive/refs/tags/10.1.1.tar.gz
    URL_HASH SHA256=78b8c0a72b1c35e4443a7e308df52498252d1cefc2b08c9a97bc9ee6cfe61f8b
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(fmt)
```

#### Git 커밋 해시로 고정
```cmake
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG 7c02e204c92545f869e2f04edaab1f19fe8b19fd  # v1.12.0의 커밋 해시
    GIT_SHALLOW OFF  # 정확한 커밋을 받기 위해
)
```

### 코드 서명 (macOS)

```cmake
if(APPLE)
    # 개발용 Ad-hoc 서명
    add_custom_command(TARGET myapp POST_BUILD
        COMMAND codesign --force --deep --sign - $<TARGET_FILE:myapp>
        COMMENT "Ad-hoc code signing"
    )
    
    # 배포용 Developer ID 서명
    if(DEFINED ENV{DEVELOPER_ID})
        add_custom_command(TARGET myapp POST_BUILD
            COMMAND codesign 
                --force 
                --deep 
                --sign "$ENV{DEVELOPER_ID}"
                --options runtime
                --timestamp
                $<TARGET_BUNDLE_DIR:myapp>
            COMMENT "Signing with Developer ID"
        )
    endif()
    
    # Hardened Runtime 설정
    set_target_properties(myapp PROPERTIES
        XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME YES
    )
endif()
```

### 정적 분석 도구 통합

#### Clang-Tidy
```cmake
option(ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)

if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    if(CLANG_TIDY_EXE)
        set(CMAKE_CXX_CLANG_TIDY 
            ${CLANG_TIDY_EXE};
            -checks=*;
            -warnings-as-errors=*;
        )
        message(STATUS "clang-tidy enabled")
    else()
        message(WARNING "clang-tidy requested but not found")
    endif()
endif()
```

#### Cppcheck
```cmake
option(ENABLE_CPPCHECK "Enable cppcheck" OFF)

if(ENABLE_CPPCHECK)
    find_program(CPPCHECK_EXE NAMES "cppcheck")
    if(CPPCHECK_EXE)
        set(CMAKE_CXX_CPPCHECK 
            ${CPPCHECK_EXE};
            --enable=warning,performance,portability;
            --suppress=missingInclude;
            --inline-suppr;
            --error-exitcode=1;
        )
        message(STATUS "cppcheck enabled")
    else()
        message(WARNING "cppcheck requested but not found")
    endif()
endif()
```

### Address Sanitizer (메모리 안전성)

```cmake
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)
option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)

if(ENABLE_ASAN)
    if(MSVC)
        target_compile_options(myapp PRIVATE /fsanitize=address)
        target_link_options(myapp PRIVATE /fsanitize=address)
    else()
        target_compile_options(myapp PRIVATE 
            -fsanitize=address 
            -fno-omit-frame-pointer
        )
        target_link_options(myapp PRIVATE -fsanitize=address)
    endif()
    message(STATUS "AddressSanitizer enabled")
endif()

if(ENABLE_UBSAN)
    if(NOT MSVC)
        target_compile_options(myapp PRIVATE 
            -fsanitize=undefined
            -fno-omit-frame-pointer
        )
        target_link_options(myapp PRIVATE -fsanitize=undefined)
        message(STATUS "UndefinedBehaviorSanitizer enabled")
    endif()
endif()

if(ENABLE_TSAN)
    if(NOT MSVC)
        target_compile_options(myapp PRIVATE 
            -fsanitize=thread
            -fno-omit-frame-pointer
        )
        target_link_options(myapp PRIVATE -fsanitize=thread)
        message(STATUS "ThreadSanitizer enabled")
    endif()
endif()
```

### 빌드 재현성 (Reproducible Builds)

```cmake
# 타임스탬프 제거로 동일한 소스에서 동일한 바이너리 생성
if(NOT MSVC)
    add_compile_options(
        -Wdate-time
        -ffile-prefix-map=${CMAKE_SOURCE_DIR}=.
    )
endif()

# 빌드 정보 기록
string(TIMESTAMP BUILD_TIMESTAMP "%Y-%m-%d %H:%M:%S UTC" UTC)
configure_file(
    ${CMAKE_SOURCE_DIR}/build_info.hpp.in
    ${CMAKE_BINARY_DIR}/build_info.hpp
)
```

`build_info.hpp.in`:
```cpp
#pragma once

constexpr const char* BUILD_TIMESTAMP = "@BUILD_TIMESTAMP@";
constexpr const char* GIT_COMMIT = "@GIT_COMMIT@";
constexpr const char* BUILD_TYPE = "@CMAKE_BUILD_TYPE@";
```

---

## 12. GitHub Actions CI/CD

### 기본 멀티플랫폼 빌드

`.github/workflows/build.yml`:
```yaml
name: Build

on:
  push:
    branches: [ main, develop ]
  pull_request:
    branches: [ main ]

jobs:
  build:
    name: ${{ matrix.config.name }}
    runs-on: ${{ matrix.config.os }}
    strategy:
      fail-fast: false
      matrix:
        config:
          - {
              name: "Windows MSVC",
              os: windows-latest,
              cc: "cl",
              cxx: "cl",
              build_type: "Release"
            }
          - {
              name: "Ubuntu GCC",
              os: ubuntu-latest,
              cc: "gcc-11",
              cxx: "g++-11",
              build_type: "Release"
            }
          - {
              name: "macOS Clang",
              os: macos-latest,
              cc: "clang",
              cxx: "clang++",
              build_type: "Release"
            }

    steps:
    - uses: actions/checkout@v4
      with:
        submodules: recursive

    - name: Install dependencies (Ubuntu)
      if: startsWith(matrix.config.os, 'ubuntu')
      run: |
        sudo apt-get update
        sudo apt-get install -y cmake ninja-build libsfml-dev

    - name: Install dependencies (macOS)
      if: startsWith(matrix.config.os, 'macos')
      run: |
        brew install cmake ninja sfml

    - name: Configure CMake
      run: |
        cmake -B build \
          -DCMAKE_BUILD_TYPE=${{ matrix.config.build_type }} \
          -DCMAKE_C_COMPILER=${{ matrix.config.cc }} \
          -DCMAKE_CXX_COMPILER=${{ matrix.config.cxx }} \
          -G Ninja

    - name: Build
      run: cmake --build build --config ${{ matrix.config.build_type }}

    - name: Test
      run: ctest --test-dir build --build-config ${{ matrix.config.build_type }}

    - name: Upload artifacts
      uses: actions/upload-artifact@v4
      with:
        name: ${{ matrix.config.name }}-build
        path: build/bin/
```

### 보안 스캔 포함

`.github/workflows/security.yml`:
```yaml
name: Security Scan

on:
  push:
    branches: [ main ]
  pull_request:
    branches: [ main ]
  schedule:
    - cron: '0 0 * * 0'  # 매주 일요일

jobs:
  codeql:
    name: CodeQL Analysis
    runs-on: ubuntu-latest
    permissions:
      security-events: write
      actions: read
      contents: read

    steps:
    - uses: actions/checkout@v4

    - name: Initialize CodeQL
      uses: github/codeql-action/init@v3
      with:
        languages: cpp

    - name: Build
      run: |
        cmake -B build -DCMAKE_BUILD_TYPE=Debug
        cmake --build build

    - name: Perform CodeQL Analysis
      uses: github/codeql-action/analyze@v3

  cppcheck:
    name: Cppcheck
    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v4

    - name: Install cppcheck
      run: sudo apt-get install -y cppcheck

    - name: Run cppcheck
      run: |
        cppcheck --enable=all \
                 --suppress=missingInclude \
                 --error-exitcode=1 \
                 --inline-suppr \
                 src/

  dependency-review:
    name: Dependency Review
    runs-on: ubuntu-latest
    if: github.event_name == 'pull_request'

    steps:
    - uses: actions/checkout@v4
    
    - name: Dependency Review
      uses: actions/dependency-review-action@v4
```

### Sanitizer 빌드

`.github/workflows/sanitizers.yml`:
```yaml
name: Sanitizers

on:
  push:
    branches: [ main, develop ]
  pull_request:
    branches: [ main ]

jobs:
  sanitize:
    name: ${{ matrix.sanitizer }}
    runs-on: ubuntu-latest
    strategy:
      fail-fast: false
      matrix:
        sanitizer: [address, undefined, thread]

    steps:
    - uses: actions/checkout@v4

    - name: Install dependencies
      run: |
        sudo apt-get update
        sudo apt-get install -y cmake ninja-build

    - name: Configure with ${{ matrix.sanitizer }}
      run: |
        cmake -B build \
          -DCMAKE_BUILD_TYPE=Debug \
          -DENABLE_$(echo ${{ matrix.sanitizer }} | tr '[:lower:]' '[:upper:]')SAN=ON \
          -G Ninja

    - name: Build
      run: cmake --build build

    - name: Test
      run: ctest --test-dir build --output-on-failure
      env:
        ASAN_OPTIONS: detect_leaks=1:check_initialization_order=1
        UBSAN_OPTIONS: print_stacktrace=1
        TSAN_OPTIONS: second_deadlock_stack=1
```

### 릴리즈 자동화

`.github/workflows/release.yml`:
```yaml
name: Release

on:
  push:
    tags:
      - 'v*'

jobs:
  create-release:
    name: Create Release
    runs-on: ubuntu-latest
    outputs:
      upload_url: ${{ steps.create_release.outputs.upload_url }}
    
    steps:
    - name: Create Release
      id: create_release
      uses: actions/create-release@v1
      env:
        GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
      with:
        tag_name: ${{ github.ref }}
        release_name: Release ${{ github.ref }}
        draft: false
        prerelease: false

  build-release:
    name: Build ${{ matrix.config.name }}
    needs: create-release
    runs-on: ${{ matrix.config.os }}
    strategy:
      matrix:
        config:
          - {
              name: "Windows",
              os: windows-latest,
              artifact_name: "myapp-windows.zip",
              asset_name: "myapp-windows-x64.zip"
            }
          - {
              name: "Ubuntu",
              os: ubuntu-latest,
              artifact_name: "myapp-linux.tar.gz",
              asset_name: "myapp-linux-x64.tar.gz"
            }
          - {
              name: "macOS",
              os: macos-latest,
              artifact_name: "myapp-macos.dmg",
              asset_name: "myapp-macos-universal.dmg"
            }

    steps:
    - uses: actions/checkout@v4

    - name: Configure
      run: |
        cmake -B build \
          -DCMAKE_BUILD_TYPE=Release \
          -DBUILD_BUNDLE=ON

    - name: Build
      run: cmake --build build --config Release

    - name: Package (Windows)
      if: startsWith(matrix.config.os, 'windows')
      run: |
        cd build
        cpack -G ZIP

    - name: Package (Linux)
      if: startsWith(matrix.config.os, 'ubuntu')
      run: |
        cd build
        cpack -G TGZ

    - name: Package (macOS)
      if: startsWith(matrix.config.os, 'macos')
      run: |
        cd build
        cpack -G DragNDrop

    - name: Upload Release Asset
      uses: actions/upload-release-asset@v1
      env:
        GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
      with:
        upload_url: ${{ needs.create-release.outputs.upload_url }}
        asset_path: build/${{ matrix.config.artifact_name }}
        asset_name: ${{ matrix.config.asset_name }}
        asset_content_type: application/octet-stream
```

### 캐싱으로 빌드 속도 향상

```yaml
name: Build with Cache

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v4

    - name: Cache CMake build
      uses: actions/cache@v4
      with:
        path: |
          build
          ~/.cache/ccache
        key: ${{ runner.os }}-build-${{ hashFiles('**/CMakeLists.txt') }}
        restore-keys: |
          ${{ runner.os }}-build-

    - name: Install ccache
      run: sudo apt-get install -y ccache

    - name: Configure
      run: |
        cmake -B build \
          -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_CXX_COMPILER_LAUNCHER=ccache

    - name: Build
      run: cmake --build build

    - name: Show ccache stats
      run: ccache -s
```

### 코드 커버리지

`.github/workflows/coverage.yml`:
```yaml
name: Code Coverage

on:
  push:
    branches: [ main ]
  pull_request:
    branches: [ main ]

jobs:
  coverage:
    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v4

    - name: Install dependencies
      run: |
        sudo apt-get update
        sudo apt-get install -y cmake lcov

    - name: Configure with coverage
      run: |
        cmake -B build \
          -DCMAKE_BUILD_TYPE=Debug \
          -DCMAKE_CXX_FLAGS="--coverage" \
          -DCMAKE_C_FLAGS="--coverage"

    - name: Build
      run: cmake --build build

    - name: Test
      run: ctest --test-dir build

    - name: Generate coverage report
      run: |
        lcov --capture --directory build --output-file coverage.info
        lcov --remove coverage.info '/usr/*' --output-file coverage.info
        lcov --list coverage.info

    - name: Upload to Codecov
      uses: codecov/codecov-action@v4
      with:
        files: ./coverage.info
        fail_ci_if_error: true
```

### Docker 빌드

`.github/workflows/docker.yml`:
```yaml
name: Docker Build

on:
  push:
    branches: [ main ]
  pull_request:
    branches: [ main ]

jobs:
  docker:
    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v4

    - name: Set up Docker Buildx
      uses: docker/setup-buildx-action@v3

    - name: Login to GitHub Container Registry
      uses: docker/login-action@v3
      with:
        registry: ghcr.io
        username: ${{ github.actor }}
        password: ${{ secrets.GITHUB_TOKEN }}

    - name: Build and push
      uses: docker/build-push-action@v5
      with:
        context: .
        push: ${{ github.event_name != 'pull_request' }}
        tags: ghcr.io/${{ github.repository }}:latest
        cache-from: type=gha
        cache-to: type=gha,mode=max
```

`Dockerfile`:
```dockerfile
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN cmake -B build -DCMAKE_BUILD_TYPE=Release -G Ninja
RUN cmake --build build

CMD ["./build/bin/myapp"]
```

### Secrets 관리 예제

```yaml
name: Build with Secrets

on: [push]

jobs:
  build:
    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v4

    - name: Configure with API key
      env:
        API_KEY: ${{ secrets.API_KEY }}
        SIGNING_KEY: ${{ secrets.CODE_SIGNING_KEY }}
      run: |
        cmake -B build \
          -DCMAKE_BUILD_TYPE=Release \
          -DAPI_KEY="${API_KEY}"

    - name: Build
      run: cmake --build build

    # Secrets는 절대 로그에 출력하지 말 것!
```

GitHub Secrets 설정:
1. Repository → Settings → Secrets and variables → Actions
2. "New repository secret" 클릭
3. Name: `API_KEY`, Secret: `your-api-key` 입력

---

## 13. 실전 예제 - 크로스 플랫폼 게임

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyGame VERSION 1.0.0 LANGUAGES CXX)

# C++ 표준
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 출력 디렉토리
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 옵션
option(BUILD_TESTS "Build tests" ON)
option(ENABLE_PROFILING "Enable profiling" OFF)

# 플랫폼 감지
if(WIN32)
    set(PLATFORM_NAME "Windows")
elseif(APPLE)
    set(PLATFORM_NAME "macOS")
elseif(UNIX)
    set(PLATFORM_NAME "Linux")
endif()

message(STATUS "Building for ${PLATFORM_NAME}")

# 외부 라이브러리
include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
)

FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
)

FetchContent_MakeAvailable(SFML spdlog)

# 소스 파일
set(COMMON_SOURCES
    src/main.cpp
    src/game.cpp
    src/renderer.cpp
)

# 플랫폼별 소스
if(WIN32)
    list(APPEND COMMON_SOURCES src/platform/windows.cpp)
elseif(APPLE)
    list(APPEND COMMON_SOURCES src/platform/macos.mm)
elseif(UNIX)
    list(APPEND COMMON_SOURCES src/platform/linux.cpp)
endif()

# 실행파일
add_executable(mygame ${COMMON_SOURCES})

# 포함 디렉토리
target_include_directories(mygame PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 링크 라이브러리
target_link_libraries(mygame PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    spdlog::spdlog
)

# 컴파일 정의
target_compile_definitions(mygame PRIVATE
    GAME_VERSION="${PROJECT_VERSION}"
    $<$<CONFIG:Debug>:DEBUG_MODE>
    $<$<BOOL:${ENABLE_PROFILING}>:ENABLE_PROFILING>
)

# 컴파일러 플래그
if(MSVC)
    target_compile_options(mygame PRIVATE /W4 /WX)
else()
    target_compile_options(mygame PRIVATE -Wall -Wextra -Werror)
endif()

# macOS 번들
if(APPLE)
    set_target_properties(mygame PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "MyGame"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.example.mygame"
    )
endif()

# 테스트
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# 설치
install(TARGETS mygame
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)
```

---

## 14. 유용한 CMake 명령어

```bash
# 빌드 디렉토리 생성 및 구성
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 빌드
cmake --build build

# 병렬 빌드
cmake --build build --parallel 8

# 특정 타겟만 빌드
cmake --build build --target myapp

# 설치
cmake --install build --prefix /usr/local

# 테스트 실행
ctest --test-dir build

# 캐시 변수 확인
cmake -L build

# 자세한 출력
cmake --build build --verbose

# 툴체인 파일 사용
cmake -B build -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake

# 빌드 클린
cmake --build build --target clean

# 전체 재설정 (빌드 디렉토리 삭제 후 재생성)
rm -rf build && cmake -B build
```

---

## 추가 팁

### CMakePresets.json 사용
최신 CMake에서는 프리셋 파일로 빌드 설정을 관리할 수 있습니다:

```json
{
  "version": 3,
  "cmakeMinimumRequired": {
    "major": 3,
    "minor": 20,
    "patch": 0
  },
  "configurePresets": [
    {
      "name": "debug",
      "displayName": "Debug",
      "description": "Debug build",
      "binaryDir": "${sourceDir}/build-debug",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug"
      }
    },
    {
      "name": "release",
      "displayName": "Release",
      "description": "Release build",
      "binaryDir": "${sourceDir}/build-release",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release"
      }
    }
  ],
  "buildPresets": [
    {
      "name": "debug",
      "configurePreset": "debug"
    },
    {
      "name": "release",
      "configurePreset": "release"
    }
  ]
}
```

사용:
```bash
cmake --preset debug
cmake --build --preset debug
```

---

## 결론

이 가이드의 내용을 마스터하면 Windows, macOS, Linux를 아우르는 멀티플랫폼 C++ 프로젝트를 효과적으로 관리할 수 있습니다. 

### 핵심 요약

1. **기본 구조**: CMake의 기본 문법과 변수 이해
2. **플랫폼 대응**: 각 OS별 특성을 고려한 빌드 설정
3. **외부 의존성**: FetchContent와 find_package로 라이브러리 관리
4. **보안**: 컴파일러 보안 플래그와 Sanitizer로 안전한 코드 작성
5. **자동화**: GitHub Actions로 CI/CD 파이프라인 구축
6. **품질 관리**: 정적 분석, 테스트, 코드 커버리지

### 다음 단계

- **작은 프로젝트부터 시작**: 간단한 프로젝트로 CMake 익히기
- **템플릿 만들기**: 자주 사용하는 설정을 템플릿화
- **문서화**: CMake 설정에 주석 달기
- **커뮤니티 활용**: GitHub에서 다른 프로젝트의 CMakeLists.txt 참고

### 유용한 리소스

- [CMake 공식 문서](https://cmake.org/documentation/)
- [Modern CMake](https://cliutils.gitlab.io/modern-cmake/)
- [Awesome CMake](https://github.com/onqtam/awesome-cmake)
- [CMake Examples](https://github.com/ttroy50/cmake-examples)

CMake의 강력한 기능들을 활용하여 유지보수가 쉽고 안전하며 확장 가능한 빌드 시스템을 구축하세요!
