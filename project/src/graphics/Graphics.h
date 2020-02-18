#pragma once
#include "TextureInterface.h"
#include "RectF.h"
#include "Size.h"

class Graphics
{
public:

	//// A single vertex with UV
	//struct Vertex 
	//{
	//	// Position in x/y plane
	//	float x, y;

	//	// UV coordinates
	//	float u, v;

	//	// Color, each channel in the range from 0-1
	//	float r, g, b, a;
	//};

	struct Color4f
	{
		Color4f(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
		Color4f() : r(0), g(0), b(0), a(0) {}

		float r;
		float g;
		float b;
		float a;
	};

	enum class BlendMode 
	{
		BLEND_NORMAL,
		BLEND_ADDITIVE,
		BLEND_MULTIPLY,
		BLEND_SCREEN
	};

	virtual void* loadTexture(const char* filePath, Size& size) = 0;
	virtual void* makeTexture(int w, int h, const void* pixels) = 0;
	virtual void draw(void* texture, const RectF& srcRect, RectF& dstRect,
		float pivotX = 0.0f, float pivotY = 0.0f,
		float angle = 0.f,
		float scaleX = 1.0f, float scaleY = 1.0f,
		bool flipX = false, bool flipY = false,
		float alpha = 1.0f,
		float red = 1.0f, float green = 1.0f, float blue = 1.0f) = 0;
	virtual void drawTriangles(float* vertices, size_t count, void* texture, BlendMode blendmode,
		float x, float y, float angle, float scaleX, float scaleY, float pivotX, float pivotY,
		Color4f tintLight, Color4f tintDark) = 0;
};
