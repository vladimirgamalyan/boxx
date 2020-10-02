find_package(Corrade REQUIRED Main)
find_package(Magnum REQUIRED GL Shaders)
find_package(CURL REQUIRED)
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
 
corrade_add_resource(Shaders_RESOURCES shaders/resources.conf)

include_directories(${CURL_INCLUDE_DIRS})

file(COPY ../assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
 
set(SOURCES
        app.cpp
        shaders/BasicShader.cpp
        shaders/BasicShaderOneColor.cpp
        shaders/TwoColorsShader.cpp
        Game.cpp
        external/lodepng.cpp
        spineIntegration.cpp
        spineTest/SpineTest.cpp
        ${Shaders_RESOURCES}
    )
	
set(INCLUDES
        app.h
        graphics/Graphics.h
        graphics/Texture.h
        graphics/RectF.h
        shaders/BasicShader.h
        shaders/BasicShaderOneColor.h
        shaders/TwoColorsShader.h
        GameEnvironment.h
        Game.h
        external/lodepng.h
        spineIntegration.h
        spineTest/SpineTest.h
	)
     
# add_subdirectory(external/spine-cpp)
file(GLOB SPINE_INCLUDES "external/spine-cpp/include/**/*.h")
file(GLOB SPINE_SOURCES "external/spine-cpp/src/**/*.cpp")
include_directories("external/spine-cpp/include")
list(APPEND SOURCES ${SPINE_SOURCES})
list(APPEND INCLUDES ${SPINE_INCLUDES})

if(CORRADE_TARGET_ANDROID)
    add_library(boxx SHARED ${SOURCES})
else()
    if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_executable(boxx WIN32 ${SOURCES})
    else()
        add_executable(boxx ${SOURCES})
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
