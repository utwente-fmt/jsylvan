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
 */
public class JSylvan
{
    /**
     * Initialize both Lace and Sylvan.
     * : number of workers, size of the work-stealing stack (Lace)
     *             2^tablesize nodes and 2^cachesize cache entries
     *             also: granularity (default: 4, sensible values: 1-10 or so) influences operations cache
     *                   higher granularity = use operations cache less often
     */
    public static void initialize(long workers, long stacksize, int tablesize, int cachesize, int granularity) throws IOException
    {
        if (instance != null) throw new RuntimeException("JSylvan already initialized!");
        instance = new JSylvan();

        initLace(workers, stacksize);
        initSylvan(1L<<tablesize, 1L<<cachesize, granularity);
    }


    public static long getTrue()
    {
        return one;
    }

    public static long getFalse()
    {
        return zero;
    }

    public static native long makeVar(int a);
    public static native long makeNot(long a);
    public static native long makeAnd(long a, long b);
    public static native long makeOr(long a, long b);
    public static native long makeImplies(long a, long b);
    public static native long makeIte(long a, long b, long c);
    public static native long makeEquals(long a, long b);
    public static native long makeNotEquals(long a, long b);
    public static native long makeExists(long a, long variables); // disjunction of variables
    public static native long makeNext(long a, long b, long variables); // paired variables - see Sylvan documentation
    public static native long makeUnionPar(long[] bdds); // create the union/disjunction of all bdds in array
    public static native long makeConstrain(long a, long b);
    public static native long makeRestrict(long a, long b);

    public static native long ref(long bdd); // returns same bdd
    public static native void deref(long bdd);
    public static native long count_refs(); // returns number of references

    public static native int getVar(long bdd);
    public static native long getIf(long bdd);
    public static native long getThen(long bdd);
    public static native long getElse(long bdd);

    public static native void print(long bdd);
    public static native void fprint(String filename, long bdd);

    public static native void printDot(long bdd);
    public static native void fprintDot(String filename, long bdd);

    public static native void disableGC();
    public static native void enableGC();

    /**
     * Calculate number of variable assignments for which the BDD evaluation yields true
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

    private static long zero;  // False
    private static long one;  // True

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

    private static native void initLace(long workers, long stacksize);
    private static native void initSylvan(long tablesize, long cachesize, int granularity);
}
