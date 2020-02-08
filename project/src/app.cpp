#include "app.h"
#include <iostream>
#include <Magnum/ImageView.h>
#include <Magnum/PixelFormat.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Buffer.h>

App::App(const Arguments& arguments) :
	Magnum::Platform::Application{ arguments, Configuration{}
		.setTitle("Test Application")
		.setSize({640, 480})
#ifdef FULLSCREEN
		.setWindowFlags(Configuration::WindowFlag::Fullscreen)
#else
		.setWindowFlags(Configuration::WindowFlag::Resizable)
#endif
	}
{
    using namespace Magnum::Math::Literals;

	vertexData = {
		{1.0f, -1.0f}, {1.0f, 0.0f},
		{1.0f, 1.0f}, {1.0f, 1.0f},
		{-1.0f, -1.0f}, {0.0f, 0.0f},
		{-1.0f, 1.0f}, {0.0f, 1.0f}
	};

	Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::FaceCulling);
	Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::DepthTest);

	_buffer.setData(vertexData, Magnum::GL::BufferUsage::DynamicDraw);

	_mesh.setPrimitive(Magnum::GL::MeshPrimitive::TriangleStrip)
		.setCount(4)
		.addVertexBuffer(_buffer, 0,
			SimpleShader::Position{},
			SimpleShader::TextureCoordinates{});

	Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::Blending);
	Magnum::GL::Renderer::setBlendEquation(Magnum::GL::Renderer::BlendEquation::Add, Magnum::GL::Renderer::BlendEquation::Add);
	Magnum::GL::Renderer::setBlendFunction(Magnum::GL::Renderer::BlendFunction::SourceAlpha, Magnum::GL::Renderer::BlendFunction::OneMinusSourceAlpha);

	setSwapInterval(1);
#if !defined(CORRADE_TARGET_EMSCRIPTEN) && !defined(CORRADE_TARGET_ANDROID)
	setMinimalLoopPeriod(16);
#endif

	projectionMatrix = Magnum::Matrix3::projection({ 800.0f, 600.0f })
		* Magnum::Matrix3::scaling({ 1.0f, -1.0f })						// flip Y-coordinate
		* Magnum::Matrix3::translation({ -400.0f, -300.0f });				// shift (0.0) to left-top corner

	gameEnvironment.graphics = this;
	game.init(&gameEnvironment);
}

void* App::makeTexture(int w, int h, const void* pixels)
{
	auto result = new Magnum::GL::Texture2D;
	int size = w * h * 4;
	Magnum::Containers::ArrayView<const void> d(pixels, size);
	auto image = Magnum::ImageView2D(Magnum::PixelFormat::RGBA8Unorm, { w, h }, d);

	result->setWrapping(Magnum::GL::SamplerWrapping::ClampToEdge)
		.setMagnificationFilter(Magnum::GL::SamplerFilter::Linear)
		.setMinificationFilter(Magnum::GL::SamplerFilter::Linear)
		.setStorage(1, Magnum::GL::TextureFormat::RGBA8, { w, h })
		.setSubImage(0, {}, image);

	return result;
}

void App::draw(void* texture, const RectF& srcRect, RectF& dstRect,
	float pivotX, float pivotY,
	float angle,
	float scaleX, float scaleY,
	bool flipX, bool flipY, 
	float alpha, float red, float green, float blue)
{
	/*
		4 2
		3 1

		right/bottom
		right/top
		left/bottom
		left/top
	*/

	Magnum::Color4 color(red, green, blue, alpha);

	float srcLeft = flipX ? srcRect.x + srcRect.w : srcRect.x;
	float srcRight = flipX ? srcRect.x : srcRect.x + srcRect.w;
	float srcTop = flipY ? srcRect.y : srcRect.y + srcRect.h;
	float srcBottom = flipY ? srcRect.y + srcRect.h : srcRect.y;

	vertexData = {
		{dstRect.w, 0},				{srcRight, srcBottom},
		{dstRect.w, dstRect.h},		{srcRight, srcTop},
		{0, 0},						{srcLeft, srcBottom},
		{0, dstRect.h},				{srcLeft, srcTop}
	};

	_buffer.setSubData(0, vertexData);

	Magnum::Matrix3 transformationMatrix = Magnum::Matrix3::translation({ dstRect.x, dstRect.y })
		* Magnum::Matrix3::rotation(Magnum::Math::Deg<Magnum::Float>(angle))
		* Magnum::Matrix3::scaling({ scaleX, scaleY })
		* Magnum::Matrix3::translation({ -pivotX, -pivotY });

	_shader.bindTexture(*((Magnum::GL::Texture2D*)texture)); //TODO: rebind only when changed
	_shader.setTransformationProjectionMatrix(projectionMatrix * transformationMatrix)
		.setColor(color)
		//.setUvMult({ 0.5f, 0.5f })
		//.setUvShift({ 0.0f, 0.5f })
		;
	_mesh.draw(_shader);
}

void App::drawEvent() 
{
	Magnum::GL::defaultFramebuffer.clear(Magnum::GL::FramebufferClear::Color);
	game.draw(this);	
    swapBuffers();
	redraw();
}

void App::tickEvent()
{
	game.update();
}
		
void App::mousePressEvent(MouseEvent& /*event*/)
{

}
		
void App::viewportEvent(ViewportEvent& /*event*/)
{
	redraw();
}

MAGNUM_APPLICATION_MAIN(App)
