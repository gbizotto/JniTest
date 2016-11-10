#include <jni.h>
#include "foo/foo.h"

JNIEXPORT jstring JNICALL
Java_br_com_ilegratest_jnitest_MainActivity_getMsgFromJni(JNIEnv *env, jobject instance) {
    return (*env)->NewStringUTF(env, "Hello From JNI");
}

JNIEXPORT jint JNICALL
Java_br_com_ilegratest_jnitest_MainActivity_getIntFromJni(JNIEnv *env, jobject instance) {
    return 10;
}

JNIEXPORT jint JNICALL
Java_br_com_ilegratest_jnitest_MainActivity_getSumFromJni(JNIEnv *env, jobject instance, jint num1,
                                                          jint num2) {
    return foo(num1 + num2);
}

JNIEXPORT jstring JNICALL
Java_br_com_ilegratest_jnitest_MainActivity_testString(JNIEnv *env, jobject instance, jstring input) {
    char *str;

    str = (*env)->GetStringUTFChars( env, input , NULL );
    printString(str);

    return (*env)->NewStringUTF(env, str);
}
