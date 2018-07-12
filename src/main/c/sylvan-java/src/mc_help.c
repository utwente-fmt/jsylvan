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

#define Abort(...) { fprintf(stderr, __VA_ARGS__); fprintf(stderr, "Abort at line %d!\n", __LINE__); return 0; }

JNIEXPORT jobject JNICALL
Java_jsylvan_MCFile_fromFile(JNIEnv *env, jclass cl, jstring filename)
{
    const char *fname = (*env)->GetStringUTFChars(env, filename, 0);
    FILE *f = fopen(fname, "r");

    if (f == NULL) {
        // TODO: exception
        Abort("Cannot open file '%s'!\n", fname);
    }

    (*env)->ReleaseStringUTFChars(env, filename, fname);

    LACE_ME;

    /* Read domain data */
    int vectorsize;
    if (fread(&vectorsize, sizeof(int), 1, f) != 1) Abort("Invalid input file!\n");

    int statebits[vectorsize];
    if (fread(statebits, sizeof(int), vectorsize, f) != (size_t)vectorsize) Abort("Invalid input file!\n");

    int actionbits;
    if (fread(&actionbits, sizeof(int), 1, f) != 1) Abort("Invalid input file!\n");

    int totalbits = 0;
    for (int i=0; i<vectorsize; i++) totalbits += statebits[i];

    printf("Read file for %d ints, %d bits per state\n", vectorsize, totalbits);

    // Read initial state
    {
        int k;
        if (fread(&k, sizeof(int), 1, f) != 1) Abort("Invalid input file!\n");
        if (k != -1) Abort("Unsupported input file!\n");
    }

    BDD initial;
    if (mtbdd_reader_frombinary(f, &initial, 1) != 0) Abort("Invalid input file!\n");
    mtbdd_ref(initial);

    // Read number of transitions
    int numberOfGroups;
    if (fread(&numberOfGroups, sizeof(int), 1, f) != 1) Abort("Invalid input file!\n");

    printf("There are %d transition groups.\n", numberOfGroups);

    // Read transition projections
    BDD domains[numberOfGroups];

    for (int z=0; z<numberOfGroups; z++) {
        int r_k, w_k;
        if (fread(&r_k, sizeof(int), 1, f) != 1) Abort("Invalid file format.");
        if (fread(&w_k, sizeof(int), 1, f) != 1) Abort("Invalid file format.");

        int r_proj[r_k];
        int w_proj[w_k];
        if (fread(r_proj, sizeof(int), r_k, f) != (size_t)r_k) Abort("Invalid file format.");
        if (fread(w_proj, sizeof(int), w_k, f) != (size_t)w_k) Abort("Invalid file format.");

        /* Compute a_proj the union of r_proj and w_proj, and a_k the length of a_proj */
        int a_proj[r_k+w_k];
        int r_i = 0, w_i = 0, a_i = 0;
        for (;r_i < r_k || w_i < w_k;) {
            if (r_i < r_k && w_i < w_k) {
                if (r_proj[r_i] < w_proj[w_i]) {
                    a_proj[a_i++] = r_proj[r_i++];
                } else if (r_proj[r_i] > w_proj[w_i]) {
                    a_proj[a_i++] = w_proj[w_i++];
                } else /* r_proj[r_i] == w_proj[w_i] */ {
                    a_proj[a_i++] = w_proj[w_i++];
                    r_i++;
                }
            } else if (r_i < r_k) {
                a_proj[a_i++] = r_proj[r_i++];
            } else if (w_i < w_k) {
                a_proj[a_i++] = w_proj[w_i++];
            }
        }
        const int a_k = a_i;

        /* Compute all_variables, which are all variables the transition relation is defined on */
        uint32_t all_vars[totalbits * 2];
        uint32_t curvar = 0; // start with variable 0
        int i=0, j=0, n=0;
        for (; i<vectorsize && j<a_k; i++) {
            if (i == a_proj[j]) {
                for (int k=0; k<statebits[i]; k++) {
                    all_vars[n++] = curvar;
                    all_vars[n++] = curvar + 1;
                    curvar += 2;
                }
                j++;
            } else {
                curvar += 2 * statebits[i];
            }
        }
        domains[z] = sylvan_set_fromarray(all_vars, n);
        mtbdd_ref(domains[z]);
    }

    // Read transitions
    BDD next[numberOfGroups];
    for (int z=0; z<numberOfGroups; z++) {
        if (mtbdd_reader_frombinary(f, &next[z], 1) != 0) Abort("Invalid input file!\n");
        mtbdd_ref(next[z]);
    }

    fclose(f);

    // Create domain
    int dom_arr[totalbits];
    for (int k=0; k<totalbits; k++) { dom_arr[k] = 2*k; }
    BDD domain = sylvan_set_fromarray(dom_arr, totalbits);
    mtbdd_ref(domain);

    // Convert everything to a nice object
    jmethodID constructor = (*env)->GetMethodID(env, cl, "<init>", "()V");
    jobject result = (*env)->NewObject(env, cl, constructor);

    jfieldID fid;
    jlongArray arr;
    jlong *dest;

    // set vectorSize field
    fid = (*env)->GetFieldID(env, cl, "vectorSize", "J");
    (*env)->SetLongField(env, result, fid, (jlong)vectorsize);

    // set totalBits field
    fid = (*env)->GetFieldID(env, cl, "totalBits", "J");
    (*env)->SetLongField(env, result, fid, (jlong)totalbits);

    // set initial field
    fid = (*env)->GetFieldID(env, cl, "initial", "J");
    (*env)->SetLongField(env, result, fid, (jlong)initial);

    // set domain field
    fid = (*env)->GetFieldID(env, cl, "domain", "J");
    (*env)->SetLongField(env, result, fid, (jlong)domain);

    // set relations field
    arr = (*env)->NewLongArray(env, numberOfGroups);
    dest = (*env)->GetLongArrayElements(env, arr, 0);
    for (int i=0; i<numberOfGroups; i++) dest[i] = next[i];
    (*env)->ReleaseLongArrayElements(env, arr, dest, 0);
    fid = (*env)->GetFieldID(env, cl, "relations", "[J");
    (*env)->SetObjectField(env, result, fid, arr);

    // set domains array
    arr = (*env)->NewLongArray(env, numberOfGroups);
    dest = (*env)->GetLongArrayElements(env, arr, 0);
    for (int i=0; i<numberOfGroups; i++) dest[i] = domains[i];
    (*env)->ReleaseLongArrayElements(env, arr, dest, 0);
    fid = (*env)->GetFieldID(env, cl, "relation_domains", "[J");
    (*env)->SetObjectField(env, result, fid, arr);

    return result;
}
