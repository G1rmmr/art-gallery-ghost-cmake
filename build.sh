#!/bin/bash

# 색상 정의
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 도움말 함수
show_help() {
    echo "Usage: ./build.sh [option]"
    echo ""
    echo "Options:"
    echo "  debug      - Debug 빌드 (로그 활성화)"
    echo "  release    - Release 빌드 (로그 비활성화)"
    echo "  bundle     - Release 앱 번들 빌드"
    echo "  clean      - 모든 빌드 디렉토리 삭제"
    echo "  run        - Debug 빌드 후 실행"
    echo "  help       - 이 도움말 표시"
    echo ""
    echo "Example:"
    echo "  ./build.sh debug"
    echo "  ./build.sh run"
}

# Debug 빌드
build_debug() {
    echo -e "${GREEN}Building Debug...${NC}"
    cmake -B build -DCMAKE_BUILD_TYPE=Debug
    cmake --build build
    echo -e "${GREEN}Debug build complete: ./build/bin/art-gallery-ghost${NC}"
}

# Release 빌드
build_release() {
    echo -e "${GREEN}Building Release...${NC}"
    cmake -B build-release -DCMAKE_BUILD_TYPE=Release
    cmake --build build-release
    echo -e "${GREEN}Release build complete: ./build-release/bin/art-gallery-ghost${NC}"
}

# Bundle 빌드
build_bundle() {
    echo -e "${GREEN}Building App Bundle...${NC}"
    cmake -B build-bundle -DCMAKE_BUILD_TYPE=Release -DBUILD_BUNDLE=ON
    cmake --build build-bundle
    echo -e "${GREEN}Bundle build complete: ./build-bundle/bin/art-gallery-ghost.app${NC}"
}

# 빌드 디렉토리 삭제
clean_all() {
    echo -e "${YELLOW}Cleaning all build directories...${NC}"
    rm -rf build build-release build-bundle
    echo -e "${GREEN}Clean complete${NC}"
}

# Debug 빌드 후 실행
build_and_run() {
    build_debug
    echo -e "${GREEN}Running...${NC}"
    clear
    ./build/bin/art-gallery-ghost
}

# 인자 처리
case "$1" in
    debug)
        build_debug
        ;;
    release)
        build_release
        ;;
    bundle)
        build_bundle
        ;;
    clean)
        clean_all
        ;;
    run)
        build_and_run
        ;;
    help|--help|-h)
        show_help
        ;;
    "")
        echo -e "${YELLOW}No option specified. Building debug...${NC}"
        build_debug
        ;;
    *)
        echo -e "${RED}Unknown option: $1${NC}"
        show_help
        exit 1
        ;;
esac
