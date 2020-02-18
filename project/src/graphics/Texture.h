#pragma once
#include <string>
#include <cstddef>
#include <Magnum/GL/Texture.h>
#include "TextureInterface.h"

class Texture : public TextureInterface, public Magnum::GL::Texture2D
{
public:
	virtual Size getSize() const;

	//void loadFromFile(const std::string& filePath);
	//void loadFromMemory(const void* data, std::size_t size);


private:

};
