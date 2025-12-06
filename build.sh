#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

show_help() {
    echo "Usage: ./build.sh [option]"
    echo ""
    echo "Options:"
    echo "  debug      - Build Debug (Log ON, Profile ON)"
    echo "  noprof     - Build Debug (Log ON, Profile OFF)"
    echo "  release    - Build Release (Log OFF, Profile OFF)"
    echo "  profile    - Build Release (Log OFF, Profile ON)"
    echo "  bundle     - Build Release (App Bundle)"
    echo "  clean      - Delete build directories"
    echo "  run        - Run After Building 'noprof'"
    echo "  help       - Show Guide"
    echo ""
    echo "Example:"
    echo "  ./build.sh debug"
    echo "  ./build.sh noprof"
    echo "  ./build.sh run"
}

build_debug() {
    echo -e "${GREEN}Building Debug...${NC}"
    cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_PROFILE=ON
    cmake --build build
    echo -e "${GREEN}Debug build complete: ./build/bin/art-gallery-ghost${NC}"
}

build_noprof() {
    echo -e "${GREEN}Building Noprof...${NC}"
    cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_PROFILE=OFF
    cmake --build build
    echo -e "${GREEN}Debug build complete: ./build/bin/art-gallery-ghost${NC}"
}

build_release() {
    echo -e "${GREEN}Building Release...${NC}"
    cmake -B build-release -DCMAKE_BUILD_TYPE=Release -DENABLE_PROFILE=OFF
    cmake --build build-release
    echo -e "${GREEN}Release build complete: ./build-release/bin/art-gallery-ghost${NC}"
}

build_profile() {
    echo -e "${GREEN}Building Profile...${NC}"
    cmake -B build-profile -DCMAKE_BUILD_TYPE=Release -DENABLE_PROFILE=ON
    cmake --build build-profile
    echo -e "${GREEN}Profile build complete: ./build-profile/bin/art-gallery-ghost${NC}"
}

build_bundle() {
    echo -e "${GREEN}Building Bundle...${NC}"
    cmake -B build-bundle -DCMAKE_BUILD_TYPE=Release -DBUILD_BUNDLE=ON -DENABLE_PROFILE=OFF
    cmake --build build-bundle
    echo -e "${GREEN}Bundle build complete: ./build-bundle/bin/art-gallery-ghost.app${NC}"
}

clean_all() {
    echo -e "${YELLOW}Cleaning...${NC}"
    rm -rf build build-release build-profile build-bundle
    echo -e "${GREEN}Clean complete${NC}"
}

build_and_run() {
    build_noprof
    echo -e "${GREEN}Running...${NC}"
    clear
    ./build/bin/art-gallery-ghost
}

profile_and_run() {
    build_profile
    echo -e "${GREEN}Running with profiling...${NC}"
    clear
    ./build-profile/bin/art-gallery-ghost
}

case "$1" in
    debug)
        build_debug
        ;;
    noprof)
        build_noprof
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
        echo -e "${YELLOW}No option specified. Building noprof by default...${NC}"
        build_noprof
        ;;
    *)
        echo -e "${RED}Unknown option: $1${NC}"
        show_help
        exit 1
        ;;
esac
