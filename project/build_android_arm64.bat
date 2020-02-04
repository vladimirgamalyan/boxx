set cmake="C:/Program Files/CMake/bin/cmake"
set corrade_rc=%~dp0/corrade-rc.exe
set PATH=%PATH%;%~dp0/ninja
set install_prefix=%~dp0/output
set build_dir=build_android_arm64
set android_arch_abi=arm64-v8a
set ndk_root=C:/android-ndk-r21
set android_version=24
set cmake_install_prefix=%ndk_root%/platforms/android-%android_version%/arch-arm64/usr

cd /D %~dp0
rmdir /S /Q %build_dir%
mkdir %build_dir%
cd %build_dir%

%cmake% .. ^
	-G Ninja ^
    -DCMAKE_SYSTEM_NAME=Android ^
	-DCORRADE_TARGET_ANDROID=ON ^
    -DCMAKE_SYSTEM_NAME=Android ^
    -DCMAKE_SYSTEM_VERSION=%android_version% ^
    -DCMAKE_ANDROID_ARCH_ABI=%android_arch_abi% ^
    -DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang ^
    -DCMAKE_ANDROID_STL_TYPE=c++_static ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_INSTALL_PREFIX=%cmake_install_prefix% ^
    -DCORRADE_RC_EXECUTABLE=%corrade_rc% ^
	-DCMAKE_ANDROID_NDK=%ndk_root% ^
	-DANDROID_PLATFORM_VERSION=%android_version%

%cmake% --build .


cd /D %~dp0
