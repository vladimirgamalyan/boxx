#include "app.h"
#include <Magnum/GL/DefaultFramebuffer.h>

MyApplication::MyApplication(const Arguments& arguments): Magnum::Platform::Application{arguments}
{

}

void MyApplication::drawEvent() 
{
    Magnum::GL::defaultFramebuffer.clear(Magnum::GL::FramebufferClear::Color);

    swapBuffers();
}

MAGNUM_APPLICATION_MAIN(MyApplication)
