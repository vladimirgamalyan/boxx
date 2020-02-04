#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Shaders/VertexColor.h>

#ifdef CORRADE_TARGET_ANDROID
#include <Magnum/Platform/AndroidApplication.h>
#elif defined(CORRADE_TARGET_EMSCRIPTEN)
#include <Magnum/Platform/EmscriptenApplication.h>
#else
#include <Magnum/Platform/Sdl2Application.h>
#endif

//#include <jni.h>
//#include <android/native_activity.h>

/*
#ifdef CORRADE_TARGET_ANDROID
#include <platform/android/jni/JniHelper.h>
#endif

static void runQrScanner()
{
#ifdef CORRADE_TARGET_ANDROID
	JniMethodInfo methodInfo;
	if (cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "runQrScanner", "()V"))
	{
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
#endif
}
*/

//static JavaVM* g_JavaVM = 0;
//
//JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
//{
//	g_JavaVM = vm;
//    return JNI_VERSION_1_4;
//}

namespace Magnum {
    namespace Examples {

        class TriangleExample : public Platform::Application {
        public:
            explicit TriangleExample(const Arguments& arguments);

        private:
			bool drawEnabled = true;
		
            void drawEvent() override;
			void mousePressEvent(MouseEvent& event) override;
			void viewportEvent(ViewportEvent& event) override;
			
			void popupQrScanner();

            GL::Mesh _mesh;
            Shaders::VertexColor2D _shader;
			
			int tick = 0;
        };

        TriangleExample::TriangleExample(const Arguments& arguments) :
            Platform::Application{ arguments, Configuration{}
				.setTitle("Magnum Triangle Example")
				.setSize({640, 480})
#ifndef _DEBUG
				.setWindowFlags(Configuration::WindowFlag::Fullscreen)
#endif
			}
        {
            using namespace Math::Literals;

            struct TriangleVertex {
                Vector2 position;
                Color3 color;
            };
            const TriangleVertex data[]{
                {{-0.5f, -0.5f}, 0xff0000_rgbf},    /* Left vertex, red color */
                {{ 0.5f, -0.5f}, 0x00ff00_rgbf},    /* Right vertex, green color */
                {{ 0.0f,  0.5f}, 0x0000ff_rgbf}     /* Top vertex, blue color */
            };

            GL::Buffer buffer;
            buffer.setData(data);

            _mesh.setCount(3)
                .addVertexBuffer(std::move(buffer), 0,
                    Shaders::VertexColor2D::Position{},
                    Shaders::VertexColor2D::Color3{});
        }

        void TriangleExample::drawEvent() {
            GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

			if (drawEnabled)
				_mesh.draw(_shader);
			
			if (tick)
			{
				--tick;
			}
			else
			{
				//drawEnabled = !drawEnabled;
				tick = 60;
			}	

            swapBuffers();
			
			redraw();
        }
		
		void TriangleExample::popupQrScanner() {
			//// JNIEnv *env;
			//// g_JavaVM->GetEnv((void**)&env, JNI_VERSION_1_4);
			//
			//JNIEnv *env = nativeActivity()->env;

			//jclass clazz = env->FindClass("com/boxx/game/BoxxActivity");
			//jmethodID mid = env->GetStaticMethodID(clazz, "runQrScanner", "()V");

			//env->CallStaticVoidMethod(clazz, mid);
		}
		
		void TriangleExample::mousePressEvent(MouseEvent& /*event*/) {
			drawEnabled = !drawEnabled;
			redraw();
			popupQrScanner();
		}
		
		void TriangleExample::viewportEvent(ViewportEvent& /*event*/) {
			//drawEnabled = !drawEnabled;
			redraw();
		}

    }
}

MAGNUM_APPLICATION_MAIN(Magnum::Examples::TriangleExample)
