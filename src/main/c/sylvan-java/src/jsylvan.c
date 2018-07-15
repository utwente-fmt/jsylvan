/*
 * Copyright 2014 Tom van Dijk
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include <sylvan.h>
#include <lace.h>

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
    LACE_ME;
    return sylvan_and(a, b);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeOr(JNIEnv *env, jclass cl, jlong a, jlong b)
{
    LACE_ME;
    return sylvan_or(a, b);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeIte(JNIEnv *env, jclass cl, jlong a, jlong b, jlong c)
{
    LACE_ME;
    return sylvan_ite(a, b, c);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeEquals(JNIEnv *env, jclass cl, jlong a, jlong b)
{
    LACE_ME;
    return sylvan_equiv(a, b);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeNotEquals(JNIEnv *env, jclass cl, jlong a, jlong b)
{
    LACE_ME;
    return sylvan_xor(a, b);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeExists(JNIEnv *env, jclass cl, jlong a, jlong b)
{
    LACE_ME;
    return sylvan_exists(a, b);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeNext(JNIEnv *env, jclass cl, jlong a, jlong b, jlong variables)
{
    LACE_ME;
    return sylvan_relnext(a, b, variables);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makePrev(JNIEnv *env, jclass cl, jlong a, jlong b, jlong variables)
{
    LACE_ME;
    return sylvan_relprev(a, b, variables);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeClosure(JNIEnv *env, jclass cl, jlong a)
{
    LACE_ME;
    return sylvan_closure(a);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeForall(JNIEnv *env, jclass cl, jlong a, jlong b)
{
    LACE_ME;
    return sylvan_forall(a, b);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeAndExists(JNIEnv *env, jclass cl, jlong a, jlong b, jlong variables)
{
    LACE_ME;
    return sylvan_and_exists(a, b, variables);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeProject(JNIEnv *env, jclass cl, jlong a, jlong b)
{
    LACE_ME;
    return sylvan_project(a, b);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeAndProject(JNIEnv *env, jclass cl, jlong a, jlong b, jlong variables)
{
    LACE_ME;
    return sylvan_and_project(a, b, variables);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeConstrain(JNIEnv *env, jclass cl, jlong a, jlong b)
{
    LACE_ME;
    return sylvan_constrain(a, b);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeRestrict(JNIEnv *env, jclass cl, jlong a, jlong b)
{
    LACE_ME;
    return sylvan_restrict(a, b);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeImplies(JNIEnv *env, jclass cl, jlong a, jlong b)
{
    LACE_ME;
    return sylvan_imp(a, b);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeSupport(JNIEnv *env, jclass cl, jlong bdd)
{
    LACE_ME;
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

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_countRefs(JNIEnv *env, jclass cl)
{
    return sylvan_count_refs();
    (void)env;
    (void)cl;
}

JNIEXPORT jdouble JNICALL
Java_jsylvan_JSylvan_satcount(JNIEnv *env, jclass cl, jlong bdd, jlong variables)
{
    LACE_ME;
    return sylvan_satcount(bdd, variables);
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_nodecount(JNIEnv *env, jclass cl, jlong bdd)
{
    LACE_ME;
    return sylvan_nodecount(bdd); // note: unsigned/signed mismatch...
}

JNIEXPORT void JNICALL
Java_jsylvan_JSylvan_initLace(JNIEnv *env, jclass cl, jlong threads, jlong stacksize)
{
    lace_init(threads, stacksize);
    lace_startup(0, NULL, NULL);
}

JNIEXPORT void JNICALL
Java_jsylvan_JSylvan_initPackage(JNIEnv *env, jclass cl)
{
    sylvan_init_package();
    (void)env;
    (void)cl;
}

JNIEXPORT void JNICALL
Java_jsylvan_JSylvan_quit(JNIEnv *env, jclass cl)
{
    sylvan_quit();
    (void)env;
    (void)cl;
}

JNIEXPORT void JNICALL
Java_jsylvan_JSylvan_setLimits(JNIEnv *env, jclass cl, jlong memory_cap, jint table_ratio, jint initial_ratio)
{
    sylvan_set_limits(memory_cap, table_ratio, initial_ratio);
    (void)env;
    (void)cl;
}

JNIEXPORT void JNICALL
Java_jsylvan_JSylvan_setSizes(JNIEnv *env, jclass cl, jlong min_tablesize, jlong max_tablesize, jlong min_cachesize, jlong max_cachesize)
{
    sylvan_set_sizes(min_tablesize, max_tablesize, min_cachesize, max_cachesize);
    (void)env;
    (void)cl;
}

JNIEXPORT void JNICALL
Java_jsylvan_JSylvan_setGranularity(JNIEnv *env, jclass cl, jint granularity)
{
    sylvan_set_granularity(granularity);
    (void)env;
    (void)cl;
}

JNIEXPORT void JNICALL
Java_jsylvan_JSylvan_initMtbdd(JNIEnv *env, jclass cl)
{
    sylvan_init_mtbdd();

    // the JSylvan java class has static vars one and zero
    jfieldID one_field = (*env)->GetStaticFieldID(env, cl, "one", "J");
    jfieldID zero_field = (*env)->GetStaticFieldID(env, cl, "zero", "J");
    (*env)->SetStaticLongField(env, cl, one_field, sylvan_true);
    (*env)->SetStaticLongField(env, cl, zero_field, sylvan_false);
}

JNIEXPORT void JNICALL
Java_jsylvan_JSylvan_gc(JNIEnv *env, jclass cl)
{
    LACE_ME;
    sylvan_gc();
    (void)env;
    (void)cl;
}

JNIEXPORT jlong JNICALL
Java_jsylvan_Jsylvan_getTableUsed(JNIEnv *env, jclass cl)
{
    LACE_ME;
    size_t res;
    sylvan_table_usage(&res, NULL);
    return (long)res;
    (void)env;
    (void)cl;
}

JNIEXPORT jlong JNICALL
Java_jsylvan_Jsylvan_getTableSize(JNIEnv *env, jclass cl)
{
    LACE_ME;
    size_t res;
    sylvan_table_usage(NULL, &res);
    return (long)res;
    (void)env;
    (void)cl;
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
    sylvan_printdot(bdd, NULL);
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

TASK_3(BDD, union_par, BDD*, arr, int, first, int, last)
{
    if (first == last) return arr[first];

    BDD left, right;

    int mid = (first+last)/2;
    // check: last == first + 1
    if (first == mid) { // we have 2
        return CALL(sylvan_ite, arr[first], sylvan_true, arr[last], 0); // or
    }

    if (mid+1 == last) { // we have 3
        left = sylvan_ref(CALL(sylvan_ite, arr[first], sylvan_true, arr[mid], 0)); // or
        BDD result = CALL(sylvan_ite, left, sylvan_true, arr[last], 0);
        sylvan_deref(left);
        return result;
    }

    SPAWN(union_par, arr, first, mid);
    right = sylvan_ref(CALL(union_par, arr, mid+1, last));
    left = sylvan_ref(SYNC(union_par));
    BDD result = CALL(sylvan_ite, left, sylvan_true, right, 0);
    sylvan_deref(left);
    sylvan_deref(right);
    return result;
}

JNIEXPORT jlong JNICALL
Java_jsylvan_JSylvan_makeUnionPar(JNIEnv *env, jclass cl, jlongArray arr)
{
    LACE_ME;
    jsize len = (*env)->GetArrayLength(env, arr);
    if (len == 0) return (jlong)sylvan_false;

    jlong *dest = (*env)->GetLongArrayElements(env, arr, 0);
    BDD result = CALL(union_par, (BDD*)dest, 0, len-1);
    (*env)->ReleaseLongArrayElements(env, arr, dest, 0);

    return result;
}

JNIEXPORT void JNICALL
Java_jsylvan_JSylvan_disableGC(JNIEnv *env, jclass cl)
{
    sylvan_gc_disable();
    return;
    (void)env;
    (void)cl;
}

JNIEXPORT void JNICALL
Java_jsylvan_JSylvan_enableGC(JNIEnv *env, jclass cl)
{
    sylvan_gc_enable();
    return;
    (void)env;
    (void)cl;
}
