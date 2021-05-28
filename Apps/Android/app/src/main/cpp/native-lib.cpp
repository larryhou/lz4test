#include <jni.h>
#include <string>

extern "C" {
#include <lz4test.hpp>
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_larryhou_lz4test_MainActivity_jnistring(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_larryhou_lz4test_MainActivity_jnidecompress(JNIEnv *env, jobject instance,
                                                         jstring input_, jstring output_) {
    const char *input = env->GetStringUTFChars(input_, 0);
    const char *output = env->GetStringUTFChars(output_, 0);

    lz4t_progress = 0.0;
    auto success = lz4t_decompress(input, output);

    env->ReleaseStringUTFChars(input_, input);
    env->ReleaseStringUTFChars(output_, output);
    return static_cast<jboolean>(success);
}

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_larryhou_lz4test_MainActivity_jniprogress(JNIEnv *env, jobject instance)
{
    return static_cast<jdouble>(lz4t_progress);
}