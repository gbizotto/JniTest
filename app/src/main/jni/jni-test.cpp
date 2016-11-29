#include <jni.h>
#include <string.h>
#include "simulador-lib/calc_sim.h"
#include "simulador-lib/Perfil.h"



#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jstring JNICALL
Java_br_com_ilegratest_jnitest_MainActivity_enviaJson(JNIEnv *env, jobject instance,
                                                      jstring json_) {
    const char *json = env->GetStringUTFChars(json_, 0);

    // TODO

    std::string produtos = listaProdutosIndicados_(json);

    env->ReleaseStringUTFChars(json_, json);

    return env->NewStringUTF(produtos.c_str());
}



#ifdef __cplusplus
}
#endif