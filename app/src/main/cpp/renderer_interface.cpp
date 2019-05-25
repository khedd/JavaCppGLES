#include <jni.h>
#include <memory>
#include "Renderer.h"

std::unique_ptr<Renderer> mRenderer;


extern "C" {
JNIEXPORT void JNICALL
Java_com_willitwork_javacppgles_MyCppRenderer__1init(JNIEnv *env, jobject instance) {
    mRenderer->init();
}

JNIEXPORT void JNICALL
Java_com_willitwork_javacppgles_MyCppRenderer__1draw(JNIEnv *env, jobject instance) {
    mRenderer->draw();
}

/**
 * Creates renderer instance
 */
jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    mRenderer = std::unique_ptr<Renderer> { new Renderer{}};

    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_6;
}
}