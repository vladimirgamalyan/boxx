#pragma once
#include "../graphics/Graphics.h"
#include <spine/spine.h>

class SpineTest
{
public:
	void init(Graphics* graphics);
	void update();
	void draw() const;

private:
	Graphics* _graphics;
	spine::Skeleton* _skeleton;
	spine::AnimationState* animationState;
	spine::AnimationStateData* animationStateData;

	void drawSkeleton(spine::Skeleton* skeleton, float x, float y) const;

	mutable bool firstLoop = true;
};
