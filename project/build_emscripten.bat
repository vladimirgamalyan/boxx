set cmake="C:/Program Files/CMake/bin/cmake"
set emscripten=C:/emsdk/upstream/emscripten
set emscripten_sys=%emscripten%/system
set corrade_rc=%~dp0/corrade-rc.exe
set PATH=%PATH%;%~dp0/ninja
set install_prefix=%~dp0/web
set build_dir=build_emscripten

cd /D %~dp0
rmdir /S /Q %build_dir%
mkdir %build_dir%
cd %build_dir%


%cmake% .. ^
	-G Ninja ^
    -DCMAKE_TOOLCHAIN_FILE="../toolchains/generic/Emscripten-wasm.cmake" ^
	-DEMSCRIPTEN_PREFIX=%emscripten% ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_INSTALL_PREFIX=%install_prefix% ^
	-DCMAKE_PREFIX_PATH=%emscripten_sys% ^
    -DCORRADE_RC_EXECUTABLE=%corrade_rc%

%cmake% --build .
%cmake% --build . --target install


cd /D %~dp0
