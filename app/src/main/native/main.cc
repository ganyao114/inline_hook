//
// Created by swift on 2021/2/24.
//

#include "main.h"

#include <instrcache/instr_cache.h>
#include <include/inline_hook.h>
#include <jni.h>
#include <common/log.h>

using namespace SandHook::Inline;

static bool registerNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *jniMethods, int methods) {
    jclass clazz = env->FindClass(className);
    if (clazz == nullptr) {
        return false;
    }
    return env->RegisterNatives(clazz, jniMethods, methods) >= 0;
}

void (*backup)();

int b = 0;

void TestHook() {
    b++;
    b++;
    b--;
    b--;
    b--;
    b--;
    b--;
    LOGE("TestHook");
    LOGE("TestHook");
    LOGE("TestHook");
    LOGE("TestHook");
    LOGE("TestHook");
}

void Hooker1() {
    LOGE("Hooker1");
    backup();
}

void Test(JNIEnv *env, jclass clazz) {
    TestHook();
    InlineHook(TestHook, Hooker1, &backup);
    TestHook();
}

static JNINativeMethod JNIHookMethods[] = {
        {"test",                   "()V", (void *) Test}
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {

    constexpr auto  CLASS_SANDHOOK = "com/swift/sandhook/Main";

    JNIEnv *env = nullptr;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    if (!registerNativeMethods(env, CLASS_SANDHOOK, JNIHookMethods, sizeof(JNIHookMethods) / sizeof(JNINativeMethod))) {
        return -1;
    }

    return JNI_VERSION_1_6;
}

