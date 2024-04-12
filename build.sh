#!/bin/bash +x

export PROJECT_ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export PHYSX_ROOT_DIR="$PROJECT_ROOT_DIR/physx"
export PM_PxShared_PATH="$PHYSX_ROOT_DIR/../pxshared"
export PM_CMakeModules_PATH="$PHYSX_ROOT_DIR/../externals/cmakemodules"
export PM_opengllinux_PATH="$PHYSX_ROOT_DIR/../externals/opengl-linux"
export PM_TARGA_PATH="$PHYSX_ROOT_DIR/../externals/targa"
export PM_CGLINUX_PATH="$PHYSX_ROOT_DIR/../externals/cg-linux"
export PM_GLEWLINUX_PATH="$PHYSX_ROOT_DIR/../externals/glew-linux"
export PM_PATHS="$PM_opengllinux_PATH;$PM_TARGA_PATH;$PM_CGLINUX_PATH;$PM_GLEWLINUX_PATH"

sudo apt-get -y update
sudo apt-get -y upgrade
sudo apt-get install -y --no-install-recommends llvm clang build-essential

cd "$PHYSX_ROOT_DIR"
python ./buildtools/cmake_generate_projects.py $1
status=$?
if [ "$status" -ne "0" ]; then
    echo "Error $status"
    exit 1
fi
