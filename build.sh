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
    echo "  debug      - Debug 빌드 (로그 활성화, 프로파일 활성화)"
    echo "  release    - Release 빌드 (로그 비활성화, 프로파일 비활성화)"
    echo "  profile    - Release 빌드 + 프로파일 활성화"
    echo "  bundle     - Release 앱 번들 빌드"
    echo "  clean      - 모든 빌드 디렉토리 삭제"
    echo "  run        - Debug 빌드 후 실행"
    echo "  help       - 이 도움말 표시"
    echo ""
    echo "Example:"
    echo "  ./build.sh debug"
    echo "  ./build.sh profile"
    echo "  ./build.sh run"
}

# Debug 빌드
build_debug() {
    echo -e "${GREEN}Building Debug...${NC}"
    cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_PROFILE=ON
    cmake --build build
    echo -e "${GREEN}Debug build complete: ./build/bin/art-gallery-ghost${NC}"
}

# Release 빌드
build_release() {
    echo -e "${GREEN}Building Release...${NC}"
    cmake -B build-release -DCMAKE_BUILD_TYPE=Release -DENABLE_PROFILE=OFF
    cmake --build build-release
    echo -e "${GREEN}Release build complete: ./build-release/bin/art-gallery-ghost${NC}"
}

# Profile 빌드 (Release 최적화 + 프로파일링)
build_profile() {
    echo -e "${GREEN}Building Profile (Release + Profiling)...${NC}"
    cmake -B build-profile -DCMAKE_BUILD_TYPE=Release -DENABLE_PROFILE=ON
    cmake --build build-profile
    echo -e "${GREEN}Profile build complete: ./build-profile/bin/art-gallery-ghost${NC}"
}

# Bundle 빌드
build_bundle() {
    echo -e "${GREEN}Building App Bundle...${NC}"
    cmake -B build-bundle -DCMAKE_BUILD_TYPE=Release -DBUILD_BUNDLE=ON -DENABLE_PROFILE=OFF
    cmake --build build-bundle
    echo -e "${GREEN}Bundle build complete: ./build-bundle/bin/art-gallery-ghost.app${NC}"
}

# 빌드 디렉토리 삭제
clean_all() {
    echo -e "${YELLOW}Cleaning all build directories...${NC}"
    rm -rf build build-release build-profile build-bundle
    echo -e "${GREEN}Clean complete${NC}"
}

# Debug 빌드 후 실행
build_and_run() {
    build_debug
    echo -e "${GREEN}Running...${NC}"
    clear
    ./build/bin/art-gallery-ghost
}

# Profile 빌드 후 실행
profile_and_run() {
    build_profile
    echo -e "${GREEN}Running with profiling...${NC}"
    clear
    ./build-profile/bin/art-gallery-ghost
}

# 인자 처리
case "$1" in
    debug)
        build_debug
        ;;
    release)
        build_release
        ;;
    profile)
        build_profile
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
    prun)
        profile_and_run
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
