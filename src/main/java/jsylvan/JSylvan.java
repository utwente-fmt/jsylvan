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

package jsylvan;

import java.io.IOException;
import java.util.List;
import java.util.ArrayList;

/**
 * Java bindings for Sylvan.
 *
 * This is only a very basic Java-Sylvan bridge.
 * Notably unsupported is calling Sylvan from different threads.
 * The only thread that may use JSylvan is the thread that initialized JSylvan.
 *
 * A BDD is a 64-bit long integer.
 * Users must explicitly reference used BDDs using methods ref() and deref().
 *
 * Missing functions:
 * - nand, nor, invimp, diff, less
 * - compose and functions to manipulate maps
 * - sat_one (give one satisfying assignment)
 * - visitors, serialization
 */
public class JSylvan
{
    /**
     * Initialize both Lace and Sylvan.
     * @param workers The number of workers, 0 for autodetect.
     * @param maxMemory Maximum bytes for the unique table and computed table.
     * @param tableRatio How much bigger the unique table is vs computed table (see setLimits)
     * @param initialRatio How much smaller the tables are initially (see setLimits)
     * @param granularity Controls how often the cache is used (typically set between 1 and 10)
     */
    public static void init(int workers, long maxMemory, int tableRatio, int initialRatio, int granularity) throws IOException
    {
        if (instance != null) throw new RuntimeException("JSylvan already initialized!");
        instance = new JSylvan();

        initLace(workers, 0);

        setLimits(maxMemory, tableRatio, initialRatio);
        initPackage();

        setGranularity(granularity);
        initMtbdd();
    }

    /**
     * Terminate Lace and Sylvan and free memory.
     *
     * After this, you cannot use Sylvan anymore.
     */
    public static native void quit();

    /**
     * Controls the caching granularity for most BDD operations.
     */
    public static native void setGranularity(int granularity);

    /**
     * Get the BDD representing True.
     */
    public static long getTrue()
    {
        return one;
    }

    /**
     * Get the BDD representing False.
     */
    public static long getFalse()
    {
        return zero;
    }

    /**
     * Compute the BDD representing variable <a>.
     */
    public static native long makeVar(int a);

    /**
     * Given BDD representing formula <a>, compute the negation. (complement bit)
     */
    public static native long makeNot(long a);

    /**
     * Compute <a> and <b>
     */
    public static native long makeAnd(long a, long b);

    /**
     * Compute <a> or <b>.
     */
    public static native long makeOr(long a, long b);

    /**
     * Compute <a> implies <b>.
     */
    public static native long makeImplies(long a, long b);

    /**
     * Compute IF <a> THEN <b> ELSE <c>.
     */
    public static native long makeIte(long a, long b, long c);

    /**
     * Compute <a> equals <b>.
     */
    public static native long makeEquals(long a, long b);

    /**
     * Compute <a> xor <b>.
     */
    public static native long makeNotEquals(long a, long b);

    /**
     * Given set of variables encoded as BDD <variables>, compute existential quantification.
     */
    public static native long makeExists(long a, long variables);

    /**
     * Given set of variables encoded as BDD <variables>, compute universal quantification.
     */
    public static native long makeForall(long a, long variables);

    /**
     * Given set of variables encoded as BDD <variables>, project <a> unto <variables>.
     *
     * This is like Exists, but now <variables> contains variables to keep.
     */
    public static native long makeProject(long a, long variables);

    /**
     * Compute exist <variables> . <a> and <b>.
     */
    public static native long makeAndExists(long a, long b, long variables);

    /**
     * Given set of variables encoded as BDD <variables>, project <a> and <b> unto <variables>.
     *
     * This is like AndExists, but now <variables> contains variables to keep.
     */
    public static native long makeAndProject(long a, long b, long variables);

    /**
     * Compute relnext(<a> with <b> on <variables>)
     *
     * That is image computation using transition relation <b> which is on the variables in
     * the set <variables>. See also the documentation in Sylvan for relnext.
     * Note that variables are paired, with 0/2/4 in X, 1/3/5 in X'.
     */
    public static native long makeNext(long a, long b, long variables);

    /**
     * Compute relprev(<a> and <b> on <variables>)
     *
     * Important: <a> is the transition relation, <b> is the set of next states.
     * So computes the predecessors of states in <b> given relation <a>.
     * This can be used to concatenate two transition relations <a> and <b>.
     */
    public static native long makePrev(long a, long b, long variables);

    /**
     * Compute the transitive closure of the given BDD.
     *
     * See Y. Matsunaga, P. C. McGeer, R. K. Brayton
     *     On Computing the Transitive Closure of a State Transition Relation
     *     30th ACM Design Automation Conference, 1993.
     *
     * The input BDD must be a transition relation that only has levels of s,t
     * with s,t interleaved with s even and t odd, i.e.
     * s level 0,2,4 matches with t level 1,3,5 and so forth.
     */
    public static native long makeClosure(long a);

    /**
     * Given an array of BDDs, compute their disjunction/union in parallel.
     */
    public static native long makeUnionPar(long[] bdds);

    /**
     * Compute the constrain or general cofactor of f @ c.
     */
    public static native long makeConstrain(long f, long c);

    /**
     * Compute the restrict of f given care function c.
     */
    public static native long makeRestrict(long f, long c);

    /**
     * Reference the given BDD.
     * @param the BDD to ref...
     * @return the result is the given BDD.
     */
    public static native long ref(long bdd);

    /**
     * Dereference the given BDD.
     */
    public static native void deref(long bdd);

    /**
     * Count the current number of references.
     */
    public static native long countRefs();

    /**
     * Get the variable label of the root node of the BDD.
     */
    public static native int getVar(long bdd);

    /**
     * Same as getVar, but instead of the label, returns a BDD representing that variable.
     */
    public static native long getIf(long bdd);

    /**
     * Get the BDD where getVar(<bdd>) is true.
     */
    public static native long getThen(long bdd);

    /**
     * Get the BDD where getVar(<bdd>) is false.
     */
    public static native long getElse(long bdd);

    /**
     * Write the BDD as a text to standard out.
     */
    public static native void print(long bdd);

    /**
     * Write the BDD as a text to a file.
     */
    public static native void fprint(String filename, long bdd);

    /**
     * Write the BDD as a dot graph to standard out.
     */
    public static native void printDot(long bdd);

    /**
     * Write the BDD as a dot graph to a file.
     */
    public static native void fprintDot(String filename, long bdd);

    /**
     * Turn off automatic garbage collection, instead program will exit.
     */
    public static native void disableGC();

    /**
     * Turn on automatic garbage collection.
     */
    public static native void enableGC();

    /**
     * Force garbage collection now.
     *
     * Usually garage collection is run ``just-in-time'' when the table is full.
     */
    public static native void gc();

    /**
     * Get current number of BDD nodes in the hash table.
     */
    public static native long getTableUsed();

    /**
     * Get current size of the hash table (max number of nodes).
     */
    public static native long getTableSize();

    /**
     * Calculate number of variable assignments for which the BDD evaluation yields true
     * Parameters: the bdd, and a bdd representing the variables
     */
    public static native double satcount(long bdd, long variables);

    /**
     * Calculate the number of nodes in the BDD
     */
    public static native long nodecount(long bdd);

    /**
     * Calculate the set of variables used in a BDD
     */
    public static native long makeSupport(long bdd);

    /**
     * Create a BDD set (conjunction of variables) which you can use for exists and satcount
     */
    public static long makeSet(int[] variables)
    {
        long result = one;
        for (int i=0; i<variables.length; i++) {
            long var = ref(makeVar(variables[i])), old = ref(result);
            result = makeAnd(result, var);
            deref(var);
            deref(old);
        }
        return result; // this is not ref'ed, for reasons of consistency
    }

    public static int[] fromSet(long bdd)
    {
        List<Integer> temp = new ArrayList<Integer>();
        while (bdd != one) {
            if (bdd == zero) throw new RuntimeException("not a BDD set");
            temp.add(getVar(bdd));
            bdd = getThen(bdd);
        }
        int[] res = new int[temp.size()];
        for (int i=0; i<res.length; i++) res[i] = temp.get(i);
        return res;
    }

    /* Private stuff */

    private static long zero;  // the BDD representing False
    private static long one;   // the BDD representing True

    private JSylvan() throws IOException
    {
        try {
            NativeUtils.loadLibraryFromJar("/linux-x64/libsylvan-java.so");
        } catch (IOException ex) {
            ex.printStackTrace();
            throw ex;
        }
    }

    private static JSylvan instance = null;

    /**
     * Use before initialization to configure computation to set table sizes.
     * Memory cap for unique table and computed cache in bytes.
     * Table ratio: 0/1/2/3 means nodes table equal/2/4/8 as big as cache
     * Initial ratio: 0/1/2/3 means initially tables are equal/2/4/8x smaller than maximum
     */
    private static native void setLimits(long memoryCap, int tableRatio, int initialRatio);

    /**
     * Use before initialization to set sizes of hash tables.
     * Use only powers of 2.
     */
    private static native void setSizes(long minTableSize, long maxTableSize, long minCacheSize, long maxCacheSize);

    private static native void initLace(long workers, long stacksize);
    private static native void initPackage();
    private static native void initMtbdd();
}
