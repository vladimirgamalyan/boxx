#pragma 
#include <spine/TextureLoader.h>
#include "graphics/Graphics.h"

class MyTextureLoader : public spine::TextureLoader
{
public:
    void assignGraphics(Graphics* graphics);

    virtual void load(spine::AtlasPage& page, const spine::String& path);
    virtual void unload(void* texture);

private:
    Graphics* _graphics;
};
