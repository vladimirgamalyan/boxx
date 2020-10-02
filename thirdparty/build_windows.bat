:: !!! Remove all previous include headers from output dir (instead old configure.h from that dir will be used).
:: !!! Update cmake_minimum_required in all root CMakeLists.txt-s to cmake_minimum_required(VERSION 3.15)

set cmake="C:\Program Files\CMake\bin\cmake"
set install_prefix=%~dp0\sys
set visual_studio="Visual Studio 15 2017"
set arch="Win32"
set toolset="v141"
set msvc_runtime_library="MultiThreaded$<$<CONFIG:Debug>:Debug>"
set cmake_args=-G %visual_studio% -A %arch% -T %toolset% -DCMAKE_INSTALL_PREFIX=%install_prefix% -DCMAKE_MSVC_RUNTIME_LIBRARY=%msvc_runtime_library%

rmdir /S /Q %install_prefix%


:: ====================== SDL2 ======================

cd /D %~dp0
cd SDL2-2.0.12
rmdir /S /Q build
mkdir build
cd build

%cmake% %cmake_args% ..
%cmake% --build . --target install --config Debug
%cmake% --build . --target install --config Release

:: ==================== CORRADE =====================

cd /D %~dp0
cd corrade-2020.06
rmdir /S /Q build
mkdir build
cd build
%cmake% %cmake_args% -DBUILD_STATIC=ON ..
%cmake% --build . --target install --config Debug
%cmake% --build . --target install --config Release

:: ==================== MAGNUM ======================

cd /D %~dp0
cd magnum-2020.06
rmdir /S /Q build
mkdir build
cd build
%cmake% %cmake_args% -DBUILD_STATIC=ON -DWITH_SDL2APPLICATION=1 ..
%cmake% --build . --target install --config Debug
%cmake% --build . --target install --config Release
