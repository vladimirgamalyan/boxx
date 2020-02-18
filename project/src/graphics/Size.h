#pragma once
#include <cstdint>

class Size
{
public:
	Size() : w(0), h(0) {}
	Size(uint32_t w, uint32_t h) : w(w), h(h) {}

	uint32_t w;
	uint32_t h;
};
