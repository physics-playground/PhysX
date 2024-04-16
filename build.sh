#!/bin/bash +x

PROJECT_ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export PROJECT_ROOT_DIR

export PHYSX_ROOT_DIR="$PROJECT_ROOT_DIR/physx"
export PM_PxShared_PATH="$PROJECT_ROOT_DIR/pxshared"
export PM_CMakeModules_PATH="$PROJECT_ROOT_DIR/externals/cmakemodules"
export PM_opengllinux_PATH="$PROJECT_ROOT_DIR/externals/opengl-linux"
export PM_TARGA_PATH="$PROJECT_ROOT_DIR/externals/targa"
export PM_CGLINUX_PATH="$PROJECT_ROOT_DIR/externals/cg-linux"
export PM_GLEWLINUX_PATH="$PROJECT_ROOT_DIR/externals/glew-linux"
export PM_PATHS="$PM_opengllinux_PATH;$PM_TARGA_PATH;$PM_CGLINUX_PATH;$PM_GLEWLINUX_PATH"

if ! command -v clang &>/dev/null && ! command -v gcc &>/dev/null; then
    sudo apt-get -y update
    sudo apt-get -y upgrade
    sudo apt-get install -y --no-install-recommends llvm clang build-essential
fi

(
    cd "$PHYSX_ROOT_DIR" || true
    rm -rf "$PROJECT_ROOT_DIR/physx/compiler/linux-*/"
    python "$PHYSX_ROOT_DIR/buildtools/cmake_generate_projects.py" "$@"
    status=$?
    if [ "$status" -ne "0" ]; then
        echo "Error $status"
        exit 1
    fi

    cd "$PROJECT_ROOT_DIR/physx/compiler/linux-release" || true
    # cmake --build "$PROJECT_ROOT_DIR/physx/compiler/linux-release" --config Release
    make -j"$(nproc)"
)
