@echo on
goto:$Main

:$Main
setlocal EnableDelayedExpansion
    set "_root=%~dp0"
    if "%_root:~-1%"=="\" set "_root=%_root:~0,-1%"
    set "_args=%*"
    if "%~1."=="." set "_args=vc17win64"

    set "PHYSX_SOURCE_ROOT_DIR=%_root%"
    set "PHYSX_SOURCE_ROOT_DIR=%PHYSX_SOURCE_ROOT_DIR:\=/%"
    set "PHYSX_ROOT_DIR=%PHYSX_SOURCE_ROOT_DIR%/physx"
    set "PHYSX_ROOT_DIR=%PHYSX_ROOT_DIR:\=/%"
    set "PM_VSWHERE_PATH=%PHYSX_SOURCE_ROOT_DIR%/externals/VsWhere"
    set "PM_CMAKEMODULES_PATH=%PHYSX_SOURCE_ROOT_DIR%/externals/CMakeModules"
    set "PM_PXSHARED_PATH=%PHYSX_SOURCE_ROOT_DIR%/pxshared"
    set "PM_TARGA_PATH=%PHYSX_SOURCE_ROOT_DIR%/externals/targa"
    set "PM_PATHS=%PM_CMAKEMODULES_PATH%;%PM_TARGA_PATH%"

    if "%~1"=="clean" goto:$MainClean
    goto:$MainBuild

    :$MainClean
        if exist "%PHYSX_SOURCE_ROOT_DIR%\build" rmdir /s /q "%PHYSX_SOURCE_ROOT_DIR%\build"
        goto:$MainBuild

    ::
    :: call "%_root%\physx\generate_projects.bat" %_args%
    ::
    :$MainBuild
        set "VCPKG_ROOT=%USERPROFILE%/.vcpkg-clion/vcpkg"

        cmake ^
            -S "%PHYSX_SOURCE_ROOT_DIR%/physx/compiler/public" ^
            -B "%PHYSX_SOURCE_ROOT_DIR%/build" ^
            -Ax64 ^
            -DTARGET_BUILD_PLATFORM=windows ^
            -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake" ^
            -DPX_OUTPUT_ARCH=x86 ^
            --no-warn-unused-cli  ^
            -DPXSHARED_PATH=%PHYSX_SOURCE_ROOT_DIR%/pxshared ^
            -DPHYSX_ROOT_DIR="%PHYSX_SOURCE_ROOT_DIR%/physx" ^
            -DPX_OUTPUT_LIB_DIR="%PHYSX_SOURCE_ROOT_DIR%/physx" ^
            -DPX_OUTPUT_BIN_DIR="%PHYSX_SOURCE_ROOT_DIR%/physx" ^
            -DCMAKE_INSTALL_PREFIX="%PHYSX_SOURCE_ROOT_DIR%/physx/install/vc17win64/PhysX"

        cmake --build "%PHYSX_SOURCE_ROOT_DIR%\build" --config Debug

    :$MainDone
endlocal & exit /b %errorlevel%
