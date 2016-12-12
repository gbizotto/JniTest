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

    std::string produtos = listaProdutosIndicados_(json);

    env->ReleaseStringUTFChars(json_, json);

    return env->NewStringUTF(produtos.c_str());
}



JNIEXPORT jstring JNICALL
Java_br_com_ilegratest_jnitest_MainActivity_gravaPremio(JNIEnv *env, jobject instance,
                                                        jstring jsonPerfil_, jstring jsonProduto_) {
    const char *jsonPerfil = env->GetStringUTFChars(jsonPerfil_, 0);
    const char *jsonProduto = env->GetStringUTFChars(jsonProduto_, 0);


    std::string premio = gravaValorPremio_(jsonPerfil, jsonProduto);

    env->ReleaseStringUTFChars(jsonPerfil_, jsonPerfil);
    env->ReleaseStringUTFChars(jsonProduto_, jsonProduto);

    return env->NewStringUTF(premio.c_str());
}




#ifdef __cplusplus
}
#endif