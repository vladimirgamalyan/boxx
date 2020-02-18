#include "app.h"
#include <iostream>
#include <Magnum/ImageView.h>
#include <Magnum/PixelFormat.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Buffer.h>

#include "utils/file2vec.h"
#include "external/lodepng.h"

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

	vertexDataOneColor = {
		{1.0f, -1.0f}, {1.0f, 0.0f},
		{1.0f, 1.0f}, {1.0f, 1.0f},
		{-1.0f, -1.0f}, {0.0f, 0.0f},
		{-1.0f, 1.0f}, {0.0f, 1.0f},

		{-1.0f, 1.0f}, {0.0f, 1.0f},
		{-1.0f, 1.0f}, {0.0f, 1.0f}
	};


	vertexDataOneColor.clear();
	for (int i = 0; i < 8 * 1024; ++i)
	{
		vertexDataOneColor.push_back(Magnum::Vector2());
		vertexDataOneColor.push_back(Magnum::Vector2());
	}

	vertexData = {
		{{1.0f, -1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
		{{1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
		{{-1.0f, -1.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
		{{-1.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
		{{-1.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
		{{-1.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}
	};

	Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::FaceCulling);
	Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::DepthTest);

	_bufferOneColor.setData(vertexDataOneColor, Magnum::GL::BufferUsage::DynamicDraw);
	_buffer.setData(vertexData, Magnum::GL::BufferUsage::DynamicDraw);

	_mesh.setPrimitive(Magnum::GL::MeshPrimitive::TriangleStrip)
		.setCount(4)
		.addVertexBuffer(_buffer, 0,
			BasicShader::Position{},
			BasicShader::TextureCoordinates{},
			BasicShader::Color{}
	);

	//_meshBasicOneColor.setPrimitive(Magnum::GL::MeshPrimitive::TriangleStrip)
	//	.setCount(4)
	//	.addVertexBuffer(_bufferOneColor, 0,
	//		BasicShader::Position{},
	//		BasicShader::TextureCoordinates{}
	//);

	_meshBasicOneColorTriangles.setPrimitive(Magnum::GL::MeshPrimitive::Triangles)
		.setCount(vertexDataOneColor.size())
		.addVertexBuffer(_bufferOneColor, 0,
			BasicShader::Position{},
			BasicShader::TextureCoordinates{}
	);

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

void* App::loadTexture(const char* filePath, Size& size)
{
	std::vector<unsigned char> pngImage;
	unsigned w = 0;
	unsigned h = 0;
	unsigned error = lodepng::decode(pngImage, w, h, filePath);

	if (error)
		std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

	size.w = w;
	size.h = h;

	return makeTexture(w, h, pngImage.data());
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

	//vertexDataOneColor = {
	//	{dstRect.w, 0},				{srcRight, srcBottom},
	//	{dstRect.w, dstRect.h},		{srcRight, srcTop},
	//	{0, 0},						{srcLeft, srcBottom},
	//	{0, dstRect.h},				{srcLeft, srcTop}
	//};


	float t[2 * 3 * 4] = {
		dstRect.w, 0,				srcRight, srcBottom,
		dstRect.w, dstRect.h,		srcRight, srcTop,
		0, 0,						srcLeft, srcBottom,

		0, dstRect.h,				srcLeft, srcTop,
		0, 0,						srcLeft, srcBottom,
		dstRect.w, dstRect.h,		srcRight, srcTop
	};

	Color4f tintLight(red, green, blue, alpha);
	Color4f tintDark;
	tintDark.a = 1.f;

	drawTriangles(t, 2, texture, Graphics::BlendMode::BLEND_NORMAL,
		dstRect.x, dstRect.y, angle, scaleX, scaleY, pivotX, pivotY, tintLight, tintDark);
	return;

	vertexDataOneColor = {
		{dstRect.w, 0},				{srcRight, srcBottom},
		{dstRect.w, dstRect.h},		{srcRight, srcTop},
		{0, 0},						{srcLeft, srcBottom},

		{0, dstRect.h},				{srcLeft, srcTop},
		{0, 0},						{srcLeft, srcBottom},
		{dstRect.w, dstRect.h},		{srcRight, srcTop}
	};


	//vertexData = {
	//	{{dstRect.w, 0},				{srcRight, srcBottom},		color},
	//	{{dstRect.w, dstRect.h},		{srcRight, srcTop},			color},
	//	{{0, 0},						{srcLeft, srcBottom},		color},
	//	{{0, dstRect.h},				{srcLeft, srcTop},			color}
	//};

	_bufferOneColor.setSubData(0, vertexDataOneColor);

	Magnum::Matrix3 transformationMatrix = Magnum::Matrix3::translation({ dstRect.x, dstRect.y })
		* Magnum::Matrix3::rotation(Magnum::Math::Deg<Magnum::Float>(angle))
		* Magnum::Matrix3::scaling({ scaleX, scaleY })
		* Magnum::Matrix3::translation({ -pivotX, -pivotY });

	/*
	_shader.bindTexture(*((Magnum::GL::Texture2D*)texture)); //TODO: rebind only when changed
	_shader.setTransformationProjectionMatrix(projectionMatrix * transformationMatrix)
		//.setColor(color)
		//.setUvMult({ 0.5f, 0.5f })
		//.setUvShift({ 0.0f, 0.5f })
		;
	_mesh.draw(_shader);
	*/

	_shaderOneColor.bindTexture(*((Magnum::GL::Texture2D*)texture)); //TODO: rebind only when changed
	_shaderOneColor.setTransformationProjectionMatrix(projectionMatrix * transformationMatrix)
		.setColor(color)
		//.setUvMult({ 0.5f, 0.5f })
		//.setUvShift({ 0.0f, 0.5f })
		;
	//_meshBasicOneColor.draw(_shaderOneColor);
	_meshBasicOneColorTriangles.setCount(6);
	_meshBasicOneColorTriangles.draw(_shaderOneColor);
}

void App::drawTriangles(float* vertices, size_t count, void* texture, BlendMode blendmode,
	float x, float y, float angle, float scaleX, float scaleY, float pivotX, float pivotY,
	Color4f tintLight, Color4f tintDark)
{
	/*
		4 2
		3 1

		right/bottom
		right/top
		left/bottom
		left/top
	*/

	//std::cout << "drawTriangles " << count << std::endl;

	Magnum::Color4 colorLight(tintLight.r, tintLight.g, tintLight.b, tintLight.a);
	Magnum::Color4 colorDark(tintDark.r, tintDark.g, tintDark.b, tintDark.a);

	//vertexDataOneColor = {
	//	{dstRect.w, 0},				{srcRight, srcBottom},
	//	{dstRect.w, dstRect.h},		{srcRight, srcTop},
	//	{0, 0},						{srcLeft, srcBottom},
	//	{0, dstRect.h},				{srcLeft, srcTop}
	//};

	_bufferOneColor.setSubData(0, Magnum::Containers::ArrayView<const float>(vertices, count * 3 * 4));
	//_bufferOneColor.setSubData(0, vertexDataOneColor);

	Magnum::Matrix3 transformationMatrix = Magnum::Matrix3::translation({ x, y })
		* Magnum::Matrix3::rotation(Magnum::Math::Deg<Magnum::Float>(angle))
		* Magnum::Matrix3::scaling({ scaleX, scaleY })
		* Magnum::Matrix3::translation({ -pivotX, -pivotY });

	_twoColorsShader.bindTexture(*((Magnum::GL::Texture2D*)texture)); //TODO: rebind only when changed
	_twoColorsShader.setTransformationProjectionMatrix(projectionMatrix * transformationMatrix)
		.setLight(colorLight)
		.setDark(colorDark);
	_meshBasicOneColorTriangles.setCount(count * 3);
	_meshBasicOneColorTriangles.draw(_twoColorsShader);
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
