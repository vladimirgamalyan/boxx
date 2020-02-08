#include "app.h"
#include <iostream>

App::App(const Arguments& arguments) :
	Magnum::Platform::Application{ arguments, Configuration{}
		.setTitle("Magnum Triangle Example")
		.setSize({640, 480})
#ifdef FULLSCREEN
		.setWindowFlags(Configuration::WindowFlag::Fullscreen)
#else
		.setWindowFlags(Configuration::WindowFlag::Resizable)
#endif
	}
{
    using namespace Magnum::Math::Literals;

    struct TriangleVertex {
        Magnum::Vector2 position;
        Magnum::Color3 color;
    };

    const TriangleVertex data[]{
        {{-0.5f, -0.5f}, 0xff0000_rgbf},    /* Left vertex, red color */
        {{ 0.5f, -0.5f}, 0x00ff00_rgbf},    /* Right vertex, green color */
        {{ 0.0f,  0.5f}, 0x0000ff_rgbf}     /* Top vertex, blue color */
    };

	Magnum::GL::Buffer buffer;
    buffer.setData(data);

    _mesh.setCount(3)
        .addVertexBuffer(std::move(buffer), 0,
			Magnum::Shaders::VertexColor2D::Position{},
			Magnum::Shaders::VertexColor2D::Color3{});
}

void App::drawEvent() 
{
	Magnum::GL::defaultFramebuffer.clear(Magnum::GL::FramebufferClear::Color);
	_mesh.draw(_shader);
			
    swapBuffers();
	redraw();
}
		
void App::mousePressEvent(MouseEvent& /*event*/)
{

}
		
void App::viewportEvent(ViewportEvent& /*event*/)
{
	redraw();
}

MAGNUM_APPLICATION_MAIN(App)
