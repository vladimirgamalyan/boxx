#pragma once
#include "GameEnvironment.h"
#include "spineTest/SpineTest.h"

class Game
{
public:
	void init(GameEnvironment* env);
	void update();
	void draw(Graphics* painter) const;

private:
	GameEnvironment* _env;
	void* texture0;
	float angle;
	SpineTest spineTest;
};
