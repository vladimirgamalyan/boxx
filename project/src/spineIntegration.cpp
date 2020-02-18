#include "spineIntegration.h"
#include <spine/Extension.h>
#include <spine/spine.h>

spine::SpineExtension* spine::getDefaultExtension()
{
    return new spine::DefaultSpineExtension();
}

void MyTextureLoader::assignGraphics(Graphics* graphics)
{
    _graphics = graphics;
}

void MyTextureLoader::load(spine::AtlasPage& page, const spine::String& path)
{
    Size size;
    void* texture = _graphics->loadTexture(path.buffer(), size);

    // if texture loading failed, we simply return.
    if (!texture) 
        return;

    //Magnum::GL::Texture2D t = static_cast<Magnum::GL::Texture2D>(texture);

    //if (page.magFilter == TextureFilter_Linear) texture->setSmooth(true);
    //if (page.uWrap == TextureWrap_Repeat && page.vWrap == TextureWrap_Repeat) texture->setRepeated(true);


    // store the Texture on the rendererObject so we can
    // retrieve it later for rendering.
    page.setRendererObject(texture);

    // store the texture width and height on the spAtlasPage
    // so spine-c can calculate texture coordinates for
    // rendering.
    page.width = size.w;
    page.height = size.h;
}

void MyTextureLoader::unload(void* texture)
{
    delete texture;
}
