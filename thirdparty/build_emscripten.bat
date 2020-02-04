set cmake="C:/Program Files/CMake/bin/cmake"
set emscripten=C:/emsdk/upstream/emscripten
set emscripten_sys=%emscripten%/system
set corrade_rc=%~dp0/sys/bin/corrade-rc.exe
set PATH=%PATH%;%~dp0/ninja

:: ==================== CORRADE =====================

cd /D %~dp0
cd corrade-2019.10
rmdir /S /Q build_emscripten
mkdir build_emscripten
cd build_emscripten

%cmake% .. ^
	-G Ninja ^
    -DCMAKE_TOOLCHAIN_FILE="../toolchains/generic/Emscripten-wasm.cmake" ^
	-DEMSCRIPTEN_PREFIX=%emscripten% ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_INSTALL_PREFIX=%emscripten_sys% ^
    -DCORRADE_RC_EXECUTABLE=%corrade_rc%

%cmake% --build .
%cmake% --build . --target install


:: ==================== MAGNUM =====================

cd /D %~dp0
cd magnum-2019.10
rmdir /S /Q build_emscripten
mkdir build_emscripten
cd build_emscripten

%cmake% .. ^
	-G Ninja ^
    -DCMAKE_TOOLCHAIN_FILE="../toolchains/generic/Emscripten-wasm.cmake" ^
	-DEMSCRIPTEN_PREFIX=%emscripten% ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_INSTALL_PREFIX=%emscripten_sys% ^
    -DCORRADE_RC_EXECUTABLE=%corrade_rc% ^
    -DCMAKE_PREFIX_PATH=%emscripten_sys% ^
    -DMAGNUM_DEPLOY_PREFIX=%~dp0/tmp ^
    -DWITH_SDL2APPLICATION=ON

%cmake% --build .
%cmake% --build . --target install



cd /D %~dp0
