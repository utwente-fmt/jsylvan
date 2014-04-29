#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include <sylvan.h>

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeTrue(JNIEnv *env, jclass cl)
{
    return sylvan_true;
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeFalse(JNIEnv *env, jclass cl)
{
    return sylvan_false;
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeVar(JNIEnv *env, jclass cl, jint var)
{
    return sylvan_ithvar(var);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeNot(JNIEnv *env, jclass cl, jlong bdd)
{
    return sylvan_not(bdd);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeAnd(JNIEnv *env, jclass cl, jlong a, jlong b)
{
    return sylvan_and(a, b);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeOr(JNIEnv *env, jclass cl, jlong a, jlong b)
{
    return sylvan_or(a, b);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeIte(JNIEnv *env, jclass cl, jlong a, jlong b, jlong c)
{
    return sylvan_ite(a, b, c);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeEquals(JNIEnv *env, jclass cl, jlong a, jlong b)
{
    return sylvan_equiv(a, b);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeNotEquals(JNIEnv *env, jclass cl, jlong a, jlong b)
{
    return sylvan_xor(a, b);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeExists(JNIEnv *env, jclass cl, jlong a, jlong b)
{
    return sylvan_exists(a, b);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeImplies(JNIEnv *env, jclass cl, jlong a, jlong b)
{
    return sylvan_imp(a, b);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSYlvan_makeSupport(JNIEnv *env, jclass cl, jlong bdd)
{
    return sylvan_support(bdd);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_getIf(JNIEnv *env, jclass cl, jlong bdd)
{
    return sylvan_ithvar(sylvan_var(bdd));
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_getThen(JNIEnv *env, jclass cl, jlong bdd)
{
    return sylvan_high(bdd);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_getElse(JNIEnv *env, jclass cl, jlong bdd)
{
    return sylvan_low(bdd);
}

JNIEXPORT jint JNICALL
Java_jsylvan_JSylvan_getVar(JNIEnv *env, jclass cl, jlong bdd)
{
    return sylvan_var(bdd);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_ref(JNIEnv *env, jclass cl, jlong bdd)
{
    return sylvan_ref(bdd);
}

JNIEXPORT void JNICALL
Java_jsylvan_JSylvan_deref(JNIEnv *env, jclass cl, jlong bdd)
{
    sylvan_deref(bdd);
}

JNIEXPORT jdouble JNICALL
Java_jsylvan_JSylvan_satcount(JNIEnv *env, jclass cl, jlong bdd, jlong variables)
{
    return sylvan_satcount(bdd, variables);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_nodecount(JNIEnv *env, jclass cl, jlong bdd)
{
    return sylvan_nodecount(bdd); // note: unsigned/signed mismatch...
}

JNIEXPORT void JNICALL
Java_jsylvan_JSylvan_initLace(JNIEnv *env, jclass cl, jlong threads, jlong stacksize)
{
    lace_init(threads, stacksize, 0);
}

JNIEXPORT void JNICALL
Java_jsylvan_JSylvan_initSylvan(JNIEnv *env, jclass cl, jint tablesize, jint cachesize)
{
    sylvan_init(tablesize, cachesize, 4);

    // the JSylvan java class has static vars one and zero
    jfieldID one_field = (*env)->GetStaticFieldID(env, cl, "one", "J");
    jfieldID zero_field = (*env)->GetStaticFieldID(env, cl, "zero", "J");
    (*env)->SetStaticLongField(env, cl, one_field, sylvan_true);
    (*env)->SetStaticLongField(env, cl, zero_field, sylvan_false);
}

JNIEXPORT void JNICALL
Java_jsylvan_JSylvan_print(JNIEnv *env, jclass cl, jlong bdd)
{
    sylvan_print(bdd);
}

JNIEXPORT void JNICALL
Java_jsylvan_JSylvan_fprint(JNIEnv *env, jclass cl, jstring filename, jlong bdd)
{
    const char *path = (*env)->GetStringUTFChars(env, filename, NULL);
    FILE *f = fopen(path, "w");
    sylvan_fprint(f, bdd);
    fclose(f);
    (*env)->ReleaseStringUTFChars(env, filename, path);
}

JNIEXPORT void JNICALL
Java_jsylvan_JSylvan_printDot(JNIEnv *env, jclass cl, jlong bdd)
{
    sylvan_printdot(bdd);
}

JNIEXPORT void JNICALL
Java_jsylvan_JSylvan_fprintDot(JNIEnv *env, jclass cl, jstring filename, jlong bdd)
{
    const char *path = (*env)->GetStringUTFChars(env, filename, NULL);
    FILE *f = fopen(path, "w");
    sylvan_fprintdot(f, bdd);
    fclose(f);
    (*env)->ReleaseStringUTFChars(env, filename, path);
}


