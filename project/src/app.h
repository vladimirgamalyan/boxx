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
#include "shaders/BasicShader.h"
#include "shaders/BasicShaderOneColor.h"
#include "Game.h"

#ifdef CORRADE_TARGET_WINDOWS
#ifndef _DEBUG
#define FULLSCREEN
#endif
#endif

class App : public Magnum::Platform::Application, public Graphics {
public:
	explicit App(const Arguments& arguments);

	void* loadTexture(const char* filePath, Size& size) override;
	void* makeTexture(int w, int h, const void* pixels) override;
	void draw(void* texture, const RectF& srcRect, RectF& dstRect,
		float pivotX = 0.0f, float pivotY = 0.0f,
		float angle = 0.f,
		float scaleX = 1.0f, float scaleY = 1.0f,
		bool flipX = false, bool flipY = false,
		float alpha = 1.0f,
		float red = 1.0f, float green = 1.0f, float blue = 1.0f) override;
	void drawTriangles(float* vertices, size_t count, void* texture, BlendMode blendmode,
		float x, float y, float angle, float scaleX, float scaleY, float pivotX, float pivotY,
		float r, float g, float b, float alpha) override;

private:
	void drawEvent() override;
	void tickEvent() override;
	void mousePressEvent(MouseEvent& event) override;
	void viewportEvent(ViewportEvent& event) override;

	Magnum::GL::Buffer _buffer;
	Magnum::GL::Buffer _bufferOneColor;
	Magnum::GL::Mesh _mesh{ Magnum::NoCreate };
	Magnum::GL::Mesh _meshBasicOneColor{ Magnum::NoCreate };
	Magnum::GL::Mesh _meshBasicOneColorTriangles{ Magnum::NoCreate };
	BasicShader _shader;
	BasicShaderOneColor _shaderOneColor;
	GameEnvironment gameEnvironment;
	Game game;

	struct BasicVertex
	{
		Magnum::Vector2 pos;
		Magnum::Vector2 uv;
		Magnum::Color4 color;
	};

	std::vector<Magnum::Vector2> vertexDataOneColor;
	std::vector<BasicVertex> vertexData;
	Magnum::Matrix3 projectionMatrix;
};
