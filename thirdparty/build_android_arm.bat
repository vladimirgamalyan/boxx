set build_dir=build_android_arm
set cmake="C:/Program Files/CMake/bin/cmake"
set corrade_rc=%~dp0/sys/bin/corrade-rc.exe
set PATH=%PATH%;%~dp0/ninja
set android_arch_abi=armeabi-v7a
set ndk_root=C:/android-ndk-r21
set corrrade_include_install_prefix=%ndk_root%/sysroot/usr
set android_version=24
set cmake_install_prefix=%ndk_root%/platforms/android-%android_version%/arch-arm/usr

:: ==================== CORRADE =====================

cd /D %~dp0
cd corrade-2019.10
rmdir /S /Q %build_dir%
mkdir %build_dir%
cd %build_dir%

%cmake% .. ^
	-G Ninja ^
    -DCMAKE_SYSTEM_NAME=Android ^
    -DCMAKE_SYSTEM_VERSION=%android_version% ^
    -DCMAKE_ANDROID_ARCH_ABI=%android_arch_abi% ^
    -DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang ^
    -DCMAKE_ANDROID_STL_TYPE=c++_static ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_INSTALL_PREFIX=%cmake_install_prefix% ^
    -DCORRADE_RC_EXECUTABLE=%corrade_rc% ^
	-DCMAKE_ANDROID_NDK=%ndk_root%
	
%cmake% --build .
%cmake% --build . --target install


:: ==================== MAGNUM =====================

cd /D %~dp0
cd magnum-2019.10
rmdir /S /Q %build_dir%
mkdir %build_dir%
cd %build_dir%

%cmake% .. ^
	-G Ninja ^
    -DCMAKE_SYSTEM_NAME=Android ^
    -DCMAKE_SYSTEM_VERSION=%android_version% ^
    -DCMAKE_ANDROID_ARCH_ABI=%android_arch_abi% ^
    -DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang ^
    -DCMAKE_ANDROID_STL_TYPE=c++_static ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_INSTALL_PREFIX=%cmake_install_prefix% ^
    -DCORRADE_RC_EXECUTABLE=%corrade_rc% ^
	-DCMAKE_ANDROID_NDK=%ndk_root% ^
    -DWITH_ANDROIDAPPLICATION=ON

%cmake% --build .
%cmake% --build . --target install


cd /D %~dp0

:: -DCORRADE_INCLUDE_INSTALL_PREFIX=/opt/android-ndk/sysroot/usr ^
:: -DTARGET_GLES2=OFF ^
:: -DCORRADE_INCLUDE_INSTALL_PREFIX=%corrrade_include_install_prefix% ^
:: -DCMAKE_INSTALL_PREFIX=%cmake_install_prefix% ^