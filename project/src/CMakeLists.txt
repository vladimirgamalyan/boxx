find_package(Corrade REQUIRED Main)
find_package(Magnum REQUIRED GL  Shaders)

if(CORRADE_TARGET_ANDROID)
    find_package(Magnum REQUIRED AndroidApplication)
elseif(CORRADE_TARGET_EMSCRIPTEN)
    find_package(Magnum REQUIRED EmscriptenApplication)
else()
    find_package(Magnum REQUIRED Sdl2Application)
endif()

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

set(SOURCE_LIST
        app.cpp
        )

if(NOT CORRADE_TARGET_ANDROID)
    add_executable(boxx ${SOURCE_LIST})
else()
    add_library(boxx SHARED ${SOURCE_LIST})
endif()

target_link_libraries(boxx PRIVATE
	Corrade::Main
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
	Magnum::Shaders)

#target_link_libraries(boxx PRIVATE
#    Magnum::Application
#    Magnum::GL
#    Magnum::Magnum
#	Magnum::Shaders
#	winmm.lib
#	version.lib
#	imm32.lib
#	iphlpapi.lib
#	setupapi.lib)

# Gradle-free Android build
if(CORRADE_TARGET_ANDROID AND CMAKE_ANDROID_NDK)
    # include(${PROJECT_SOURCE_DIR}/toolchains/modules/UseAndroid.cmake)
    # android_create_apk(boxx main/AndroidManifest.xml)
endif()


if(CORRADE_TARGET_EMSCRIPTEN)
    install(TARGETS boxx DESTINATION ${CMAKE_INSTALL_PREFIX})
    install(FILES
        boxx.html
        ${MAGNUM_EMSCRIPTENAPPLICATION_JS}
        ${MAGNUM_WEBAPPLICATION_CSS}
        DESTINATION ${CMAKE_INSTALL_PREFIX})
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/boxx.js.mem
        ${CMAKE_CURRENT_BINARY_DIR}/boxx.wasm
        DESTINATION ${CMAKE_INSTALL_PREFIX} OPTIONAL)
endif()


# Make the executable a default target to build & run in Visual Studio
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT boxx)
