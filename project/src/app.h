#pragma once

#include <Magnum/Platform/Sdl2Application.h>

class MyApplication : public Magnum::Platform::Application {
public:
    explicit MyApplication(const Arguments& arguments);

private:
    void drawEvent() override;
};
