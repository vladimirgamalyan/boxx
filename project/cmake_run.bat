cd /D %~dp0
rmdir /S /Q build
mkdir build
cd build

"C:\Program Files\CMake\bin\cmake" -G "Visual Studio 15 2017" -DCMAKE_INSTALL_PREFIX="K:\projects\boxx\thirdparty\sys" -DCMAKE_TOOLCHAIN_FILE=modules\x86-windows-msvc.cmake -DSDL_STATIC=ON ..
