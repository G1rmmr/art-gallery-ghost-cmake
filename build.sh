#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

Help(){
    echo "Usage: ./build.sh [option]"
    echo ""
    echo "Options:"
    echo "  debug      - Build Debug mode (Default)"
    echo "  release    - Build Release mode"
    echo "  publish    - Build for Web (Emscripten)"
    echo "  clean      - Delete build directories"
    echo "  run        - Build Debug and Run"
    echo "  lldb       - Debug with LLDB"
    echo "  help       - Show Guide"
    echo ""
}

Debug(){
    echo -e "${GREEN}Building Debug...${NC}"
    cmake -B build -DCMAKE_BUILD_TYPE=Debug
    cmake --build build
    echo -e "${GREEN}Debug build complete: ./build/bin/art-gallery-ghost${NC}"
}

Release(){
    echo -e "${GREEN}Building Release...${NC}"
    cmake -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build
    echo -e "${GREEN}Release build complete: ./build/bin/art-gallery-ghost${NC}"
}

Publish(){
    if ! command -v emcmake &> /dev/null; then
        echo -e "${RED}Error: emcmake could not be found.${NC}"
        echo -e "${YELLOW}Please install Emscripten SDK and source the environment variables.${NC}"
        echo -e "${YELLOW}Example: source ./emsdk/emsdk_env.sh${NC}"
        exit 1
    fi

    echo -e "${GREEN}Building Web (Publish)...${NC}"
    emcmake cmake -B build-web -DCMAKE_BUILD_TYPE=Release -DEMSCRIPTEN=ON
    cmake --build build-web

    if [ $? -eq 0 ]; then
        echo -e "${GREEN}Web build complete: ./build-web/bin/art-gallery-ghost.html${NC}"
        echo -e "${YELLOW}To test locally, run: python3 -m http.server -d build-web/bin${NC}"
    else
        echo -e "${RED}Web build failed.${NC}"
        exit 1
    fi
}

Clean(){
    echo -e "${YELLOW}Cleaning...${NC}"
    rm -rf build build-web
    echo -e "${GREEN}Clean complete${NC}"
}

Run(){
    Debug
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}Running...${NC}"
        ./build/bin/art-gallery-ghost
    else
        echo -e "${RED}Build failed. Aborting run.${NC}"
    fi
}

LLDB(){
    echo -e "${GREEN}Running LLDB...${NC}"
    # Fixed path to point to build directory
    lldb build/bin/art-gallery-ghost
}

case "$1" in
    debug)
        Debug
        ;;
    release)
        Release
        ;;
    publish)
        Publish
        ;;
    clean)
        Clean
        ;;
    run)
        Run
        ;;
    lldb)
        LLDB
        ;;
    help|--help|-h)
        Help
        ;;
    "")
        echo -e "${YELLOW}No option specified. Building debug by default...${NC}"
        Debug
        ;;
    *)
        echo -e "${RED}Unknown option: $1${NC}"
        Help
        exit 1
        ;;
esac