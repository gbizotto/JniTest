#include <jni.h>
#include <string.h>
#include "simulador-lib/calc_sim.h"
#include "simulador-lib/Perfil.h"




#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jstring JNICALL
Java_br_com_ilegratest_jnitest_MainActivity_testNew(JNIEnv *env, jobject instance, jobject p) {

    // TODO
    char* test = "something";
    jclass cls = env->GetObjectClass(p);

    jfieldID fieldId = env->GetFieldID(cls, "name", "S");

    jobject s = env->GetObjectField(p, fieldId);


    char* abc = reinterpret_cast<char*>(&s);


    return env->NewStringUTF(abc);

}

JNIEXPORT jstring JNICALL
Java_br_com_ilegratest_jnitest_MainActivity_testaEnvioObjeto(JNIEnv *env, jobject instance,
                                                             jobject perfil) {

    jclass cls = env->GetObjectClass(perfil);

 //   jfieldID fieldId = env->GetFieldID(cls, "name", "S");

    jmethodID midGetName = (env)->GetMethodID(cls, "getName", "()S");
    //char nome= (env)->CallCharMethod(perfil,midGetName);

    jobject nome = (env)->CallObjectMethod(perfil,midGetName);

   // jobject s = env->GetObjectField(perfil, fieldId);


    char* abc = reinterpret_cast<char*>(&nome);


    return env->NewStringUTF(abc);
}



JNIEXPORT jstring JNICALL
Java_br_com_ilegratest_jnitest_MainActivity_testaReceberRetorno(JNIEnv *env, jobject instance) {

    // TODO


    return env->NewStringUTF("esse e o retorno");
}

JNIEXPORT void JNICALL
Java_br_com_ilegratest_jnitest_MainActivity_testString(JNIEnv *env, jobject instance,
                                                       jstring input_) {
    const char *input = env->GetStringUTFChars(input_, 0);

    // TODO

    env->ReleaseStringUTFChars(input_, input);
}



JNIEXPORT jobject JNICALL
Java_br_com_ilegratest_jnitest_MainActivity_testaBuscaObjeto(JNIEnv *env, jobject instance,
                                                             jint age) {

    jclass fulanoClass = env->FindClass("br/com/ilegratest/jnitest/Fulano");
    jmethodID midConstructor = (env)->GetMethodID(fulanoClass, "<init>", "(I)V");
    jobject employeeObject = (env)->NewObject(fulanoClass, midConstructor, age);
    return employeeObject;


}

JNIEXPORT jint JNICALL
Java_br_com_ilegratest_jnitest_MainActivity_testInt(JNIEnv *env, jobject instance, jobject fulano) {

    jclass employeeClass = (env)->GetObjectClass(fulano);
    jmethodID midGetAge = (env)->GetMethodID(employeeClass, "getAge", "()I");

    int age =  (env)->CallIntMethod(fulano, midGetAge);

    age = age *2;


    return age;

}

JNIEXPORT jstring JNICALL
Java_br_com_ilegratest_jnitest_MainActivity_testaEnvioObjetoString(JNIEnv *env, jobject instance,
                                                                   jobject fulano) {

    jstring jstr = env->NewStringUTF("This string comes from JNI");

    jclass fulanoClass = (env)->GetObjectClass(fulano);
    jmethodID midGetName = (env)->GetMethodID(fulanoClass, "getName", "Ljava/lang/String;");

    jobject result = env->CallObjectMethod(jstr, midGetName);
    const char* str = env->GetStringUTFChars((jstring) result, false);

    env->ReleaseStringUTFChars(jstr, str);


    return env->NewStringUTF(str);


}

JNIEXPORT jstring JNICALL
Java_br_com_ilegratest_jnitest_MainActivity_enviaFulano(JNIEnv *env, jobject instance,
                                                        jobject fulano, jstring name_) {
    const char *name = env->GetStringUTFChars(name_, 0);

    // TODO

    env->ReleaseStringUTFChars(name_, name);

    return env->NewStringUTF("teste");
}


JNIEXPORT void JNICALL
Java_br_com_ilegratest_jnitest_MainActivity_listaProdutosIndicados(JNIEnv *env, jobject instance) {

    // TODO
    Perfil perfil = inicializaPerfil();


}

#ifdef __cplusplus
}
#endif