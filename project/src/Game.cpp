#include "Game.h"
#include <iostream>
#include <vector>
#include "external/lodepng.h"

void Game::init(GameEnvironment* env)
{
	_env = env;

	std::vector<unsigned char> pngImage;
	unsigned w = 0;
	unsigned h = 0;
	unsigned error = lodepng::decode(pngImage, w, h, "assets/test.png");

	if (error)
		std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

	texture0 = env->graphics->makeTexture(w, h, pngImage.data());
	angle = 0.f;

	spineTest.init(env->graphics);
}
 
void Game::update()
{
	angle += 0.0002f;

	spineTest.update();
}

void Game::draw(Graphics* painter) const
{
	RectF srcRect;
	RectF dstRect;

	srcRect.x = 0.0f;
	srcRect.y = 0.0f;
	srcRect.w = 1.0f;
	srcRect.h = 1.0f;
	
	//dstRect.left = 128.0f;
	//dstRect.right = 128.0f + 256.0f;

	//dstRect.top = 64.0f + 256.0f;
	//dstRect.bottom = 64.0f + 0.0f;

	dstRect.w = 256.0f;
	dstRect.h = 256.0f;

	//dstRect.left = 0.0f;
	//dstRect.right = 256.0f;

	dstRect.x = 400.f;
	dstRect.y = 300.f;

	//painter->draw(texture0, srcRect, dstRect,
	//	128.f, 128.f,
	//	0.f,
	//	1.f, 1.f,
	//	false, false,
	//	0.9f,
	//	1.0f, 1.0f, 0.8f);

	spineTest.draw();
}
