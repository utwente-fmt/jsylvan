package jsylvan;

import java.io.IOException;
import java.util.List;
import java.util.ArrayList;

public class JSylvan
{
    /**
     * NOTE: you will have to manage references.
     * After every call to makeXXX, call ref.
     * To dereference, call deref.
     */

    /**
     * Initialize: number of workers, size of the work-stealing stack (Lace)
     *             2^tablesize nodes and 2^cachesize cache entries
     */
    public static void initialize(long workers, long stacksize, int tablesize, int cachesize)
    {
        if (instance != null) throw new RuntimeException("JSylvan already initialized!");
        instance = new JSylvan();

        initLace(workers, stacksize);
        initSylvan(tablesize, cachesize);
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

    public static native long ref(long bdd); // returns same bdd
    public static native void deref(long bdd);

    public static native int getVar(long bdd);
    public static native long getIf(long bdd);
    public static native long getThen(long bdd);
    public static native long getElse(long bdd);

    public static native void print(long bdd);
    public static native void fprint(String filename, long bdd);

    public static native void printDot(long bdd);
    public static native void fprintDot(String filename, long bdd);

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
     * Create a BDD set (disjunction of variables) which you can use for exists and satcount
     */
    public static long makeSet(int[] variables)
    {
        long result = zero;
        for (int i=0; i<variables.length; i++) {
            long var = ref(makeVar(variables[i])), old = ref(result);
            result = makeOr(result, var);
            deref(var);
            deref(old);
        }
        return result; // this is not ref'ed, for reasons of consistency
    }

    public static int[] fromSet(long bdd)
    {
        List<Integer> temp = new ArrayList<Integer>();
        while (bdd != zero) {
            if (bdd == one) throw new RuntimeException("not a BDD set");
            temp.add(getVar(bdd));
            bdd = getElse(bdd);
        }
        int[] res = new int[temp.size()];
        for (int i=0; i<res.length; i++) res[i] = temp.get(i);
        return res;
    }

    /* Private stuff */

    private static long zero;
    private static long one;

    private JSylvan()
    {
    }

    private static JSylvan instance = null;

    private static native void initLace(long workers, long stacksize);
    private static native void initSylvan(int tablesize, int cachesize);

    static {
        try {
            NativeUtils.loadLibraryFromJar("/native/libsylvan-1.so");
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }
}