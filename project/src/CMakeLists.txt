find_package(Corrade REQUIRED Main)
find_package(Magnum REQUIRED GL Shaders)
find_package(curl REQUIRED)
if(CORRADE_TARGET_ANDROID)
    find_package(Magnum REQUIRED AndroidApplication)
elseif(CORRADE_TARGET_EMSCRIPTEN)
    find_package(Magnum REQUIRED EmscriptenApplication)
else()
    find_package(Magnum REQUIRED Sdl2Application)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
 

add_definitions( -DCURL_STATICLIB )

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

include_directories(${CURL_INCLUDE_DIRS})
 
set(SOURCE_LIST
        app.cpp
        app.h
        sokol_fetch.h
    )

if(CORRADE_TARGET_ANDROID)
    add_library(boxx SHARED ${SOURCE_LIST})
else()
    if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_executable(boxx WIN32 ${SOURCE_LIST})
    else()
        add_executable(boxx ${SOURCE_LIST})
    endif()
endif()

set(LIB_LIST
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
	Magnum::Shaders
    )

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND LIB_LIST Corrade::Main)
endif()

if(CORRADE_TARGET_WINDOWS)
    list(APPEND LIB_LIST
        winmm.lib
	    version.lib
	    imm32.lib
	    iphlpapi.lib
	    setupapi.lib
        ws2_32.lib
        crypt32.lib
        wldap32.lib
        normaliz.lib
        ${CURL_LIBRARY}
    )
endif()
 
target_link_libraries(boxx PRIVATE ${LIB_LIST})

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
