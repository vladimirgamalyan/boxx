#pragma once

#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Shaders/VertexColor.h>

#ifdef CORRADE_TARGET_WINDOWS
#include <Magnum/Platform/Sdl2Application.h>
#elif defined (CORRADE_TARGET_ANDROID)
#include <Magnum/Platform/AndroidApplication.h>
#elif defined (CORRADE_TARGET_EMSCRIPTEN)
#include <Magnum/Platform/EmscriptenApplication.h>
#else
#error Unknown Target
#endif

#ifdef CORRADE_TARGET_WINDOWS
#ifndef _DEBUG
#define FULLSCREEN
#endif
#endif

class App : public Magnum::Platform::Application {
public:
	explicit App(const Arguments& arguments);

private:
	bool drawEnabled = true;

	void drawEvent() override;
	void mousePressEvent(MouseEvent& event) override;
	void viewportEvent(ViewportEvent& event) override;

	Magnum::GL::Mesh _mesh;
	Magnum::Shaders::VertexColor2D _shader;
};
