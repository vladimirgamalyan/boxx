#pragma once
#include "Texture.h"
#include "RectF.h"

class Graphics
{
public:
	virtual void* makeTexture(int w, int h, const void* pixels) = 0;
	virtual void draw(void* texture, const RectF& srcRect, RectF& dstRect,
		float pivotX = 0.0f, float pivotY = 0.0f,
		float angle = 0.f,
		float scaleX = 1.0f, float scaleY = 1.0f,
		bool flipX = false, bool flipY = false,
		float alpha = 1.0f,
		float red = 1.0f, float green = 1.0f, float blue = 1.0f) = 0;
};
