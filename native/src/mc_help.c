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
#include <sylvan.h>
#include <assert.h>
#include <jni.h>

JNIEXPORT jobject JNICALL
Java_jsylvan_MCFile_fromFile(JNIEnv *env, jclass cl, jstring filename)
{
    const char *fname = (*env)->GetStringUTFChars(env, filename, 0);
    FILE *f = fopen(fname, "r");

    if (f == NULL) {
        // TODO: exception
        fprintf(stderr, "Cannot open file '%s'!\n", fname);
        return 0;
    }

    (*env)->ReleaseStringUTFChars(env, filename, fname);

    size_t vector_size, bits_per_integer;

    // Read domain info
    assert(fread(&vector_size, sizeof(size_t), 1, f)==1);
    assert(fread(&bits_per_integer, sizeof(size_t), 1, f)==1);

    BDDVAR *buf = (BDDVAR*)alloca(sizeof(BDDVAR) * bits_per_integer * vector_size * 2);

    // Skip domain
    fseek(f, sizeof(BDDVAR) * vector_size * bits_per_integer * 2, SEEK_CUR);

    // Read initial state
    sylvan_serialize_fromfile(f);

    size_t bdd;
    assert(fread(&bdd, sizeof(size_t), 1, f) == 1);
    BDD initial = sylvan_ref(sylvan_serialize_get_reversed(bdd));

    // Read domain
    fseek(f, sizeof(size_t), SEEK_CUR);
    assert(fread(buf, sizeof(BDDVAR), bits_per_integer * vector_size, f) == bits_per_integer * vector_size);
    BDD domain = sylvan_ref(sylvan_set_fromarray(buf, bits_per_integer * vector_size));

    // Read transitions
    int nGrps;
    assert(fread(&nGrps, sizeof(int), 1, f) == 1);

    BDD *next = (BDD*)alloca(sizeof(BDD) * nGrps);
    BDD *domains = (BDD*)alloca(sizeof(BDD) * nGrps);

    int i;
    for (i=0; i<nGrps; i++) {
        sylvan_serialize_fromfile(f);

        assert(fread(&bdd, sizeof(size_t), 1, f) == 1);
        next[i] = sylvan_ref(sylvan_serialize_get_reversed(bdd));

        // Read domain
        size_t v_size;
        assert(fread(&v_size, sizeof(size_t), 1, f) == 1);
        assert(fread(buf, sizeof(BDDVAR), v_size * bits_per_integer * 2, f) == v_size * bits_per_integer * 2);
        domains[i] = sylvan_ref(sylvan_set_fromarray(buf, v_size * bits_per_integer * 2));
    }

    fclose(f);

    // Convert everything to a nice object
    jmethodID constructor = (*env)->GetMethodID(env, cl, "<init>", "()V");
    jobject result = (*env)->NewObject(env, cl, constructor);

    jfieldID fid;
    jlongArray arr;
    jlong *dest;

    // set vectorSize field
    fid = (*env)->GetFieldID(env, cl, "vectorSize", "J");
    (*env)->SetLongField(env, result, fid, (jlong)vector_size);

    // set bitsPerInteger field
    fid = (*env)->GetFieldID(env, cl, "bitsPerInteger", "J");
    (*env)->SetLongField(env, result, fid, (jlong)bits_per_integer);

    // set initial field
    fid = (*env)->GetFieldID(env, cl, "initial", "J");
    (*env)->SetLongField(env, result, fid, (jlong)initial);

    // set domain field
    fid = (*env)->GetFieldID(env, cl, "domain", "J");
    (*env)->SetLongField(env, result, fid, (jlong)domain);

    // set relations field
    arr = (*env)->NewLongArray(env, nGrps);
    dest = (*env)->GetLongArrayElements(env, arr, 0);
    for (i=0; i<nGrps; i++) dest[i] = next[i];
    (*env)->ReleaseLongArrayElements(env, arr, dest, 0);
    fid = (*env)->GetFieldID(env, cl, "relations", "[J");
    (*env)->SetObjectField(env, result, fid, arr);

    // set domains array
    arr = (*env)->NewLongArray(env, nGrps);
    dest = (*env)->GetLongArrayElements(env, arr, 0);
    for (i=0; i<nGrps; i++) dest[i] = domains[i];
    (*env)->ReleaseLongArrayElements(env, arr, dest, 0);
    fid = (*env)->GetFieldID(env, cl, "relation_domains", "[J");
    (*env)->SetObjectField(env, result, fid, arr);

    return result;
}
