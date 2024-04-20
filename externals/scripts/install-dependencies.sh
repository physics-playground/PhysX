#!/bin/bash +x

function setup() {
    if ! command -v clang &>/dev/null && ! command -v gcc &>/dev/null; then
        sudo apt-get -y update &&
            sudo apt-get -y upgrade &&
            sudo apt-get install -y --no-install-recommends \
                cmake ninja-build cmake gcovr build-essential llvm clang \
                doxygen graphviz zip ca-certificates wget unzip &&
            sudo rm -rf /var/lib/apt/lists/*
    fi

    build_wrapper_target_path=/usr/bin/build-wrapper
    if ! $build_wrapper_target_path -v &>/dev/null; then
        build_wrapper_filename=build-wrapper-linux-x86.zip
        mkdir -p ~/.build/sonarqube-wrapper &&
            cd ~/.build/sonarqube-wrapper/ &&
            wget -nc --no-check-certificate https://sonarcloud.io/static/cpp/$build_wrapper_filename &&
            unzip -q -o -d . $build_wrapper_filename &&
            rm $build_wrapper_filename &&
            sudo ln -f -s \
                ~/.build/sonarqube-wrapper/build-wrapper-linux-x86/build-wrapper-linux-x86-64 \
                $build_wrapper_target_path
    fi

    sonar_scanner_target_path=/usr/bin/sonar-scanner
    if ! $sonar_scanner_target_path -v &>/dev/null; then
        sonar_scanner_version=5.0.1.3006
        sonar_scanner_filename=sonar-scanner-cli-$sonar_scanner_version-linux.zip
        mkdir -p ~/.build/SonarScanner &&
            cd ~/.build/SonarScanner/ &&
            wget -nc --no-check-certificate https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/$sonar_scanner_filename &&
            unzip -q -o -d . $sonar_scanner_filename &&
            rm $sonar_scanner_filename &&
            sudo ln -f -s \
                ~/.build/SonarScanner/sonar-scanner-$sonar_scanner_version-linux/bin/sonar-scanner \
                sonar_scanner_target_path
    fi
}

function build() {
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

    if [ "${1:-}" == "clean" ]; then
        rm -rf .build
    fi

    if [ "${1:-}" == "manual" ]; then
        setup "$@"

        (
            rm -rf "$PROJECT_ROOT_DIR/physx/compiler/linux-*/"
            python "$PHYSX_ROOT_DIR/buildtools/cmake_generate_projects.py" "$@"
            status=$?
            if [ "$status" -ne "0" ]; then
                echo "Error $status"
                exit 1
            else
                # cmake --build "$PROJECT_ROOT_DIR/physx/compiler/linux-release" --config Release
                make --directory="$PROJECT_ROOT_DIR/physx/compiler/linux-release" -j"$(nproc)"
            fi
        )
    else
        cmake -S . -B .build/test -DCMAKE_BUILD_TYPE=Release
        cmake --build .build/test
    fi
}

build "$@"
