#pragma once
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Shaders/VertexColor.h>
#include <Magnum/Math/Matrix3.h>

#ifdef CORRADE_TARGET_WINDOWS
#include <Magnum/Platform/Sdl2Application.h>
#elif defined (CORRADE_TARGET_ANDROID)
#include <Magnum/Platform/AndroidApplication.h>
#elif defined (CORRADE_TARGET_EMSCRIPTEN)
#include <Magnum/Platform/EmscriptenApplication.h>
#else
#error Unknown Target
#endif

#include "graphics/Graphics.h"
#include "graphics/SimpleShader.h"
#include "Game.h"

#ifdef CORRADE_TARGET_WINDOWS
#ifndef _DEBUG
#define FULLSCREEN
#endif
#endif

class App : public Magnum::Platform::Application, public Graphics {
public:
	explicit App(const Arguments& arguments);

	void* App::makeTexture(int w, int h, const void* pixels) override;
	void draw(void* texture, const RectF& srcRect, RectF& dstRect,
		float pivotX = 0.0f, float pivotY = 0.0f,
		float angle = 0.f,
		float scaleX = 1.0f, float scaleY = 1.0f,
		bool flipX = false, bool flipY = false,
		float alpha = 1.0f,
		float red = 1.0f, float green = 1.0f, float blue = 1.0f) override;

private:
	void drawEvent() override;
	void tickEvent() override;
	void mousePressEvent(MouseEvent& event) override;
	void viewportEvent(ViewportEvent& event) override;

	Magnum::GL::Buffer _buffer;
	Magnum::GL::Mesh _mesh{ Magnum::NoCreate };
	SimpleShader _shader;
	GameEnvironment gameEnvironment;
	Game game;

	std::vector<Magnum::Vector2> vertexData;
	std::vector<Magnum::Vector2> vertexDataFill;
	Magnum::Matrix3 projectionMatrix;
};
