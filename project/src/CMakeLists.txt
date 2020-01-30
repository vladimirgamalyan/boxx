find_package(Magnum REQUIRED GL Shaders Sdl2Application)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_executable(boxx app.cpp app.h)

target_link_libraries(boxx PRIVATE
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
	Magnum::Shaders
	winmm.lib
	version.lib
	imm32.lib
	iphlpapi.lib
	setupapi.lib)
	
# Make the executable a default target to build & run in Visual Studio
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT boxx)
