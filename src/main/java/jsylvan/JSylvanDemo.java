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

public class JSylvanDemo
{
    public static void main(String[] args)
    {
        // to use JSylvan, initialize it...
        // 4 workers (for a 4 core machine, obviously set this to the number of cores)
        // a work-stealing stack for each worker of <default> entries
        // allocate a node table of 2^26 buckets (sorry, we did not implement resize)
        // allocate a operations cache of 2^24 entries
        try {
            JSylvan.initialize(4, 0, 26, 24, 1);
            System.out.println("Initialized Lace and Sylvan.");
        } catch (IOException ex) {
            ex.printStackTrace();
            return;
        }

        JSylvan.disableGC();
        JSylvan.enableGC();

        // All Lace/Sylvan memory is allocated using mmap.
        // The work-stealing stack can be huge, since real memory is not allocated until actually used.
        // Do not allocate a larger node table / operations cache than you can afford.
        // Currently, 2^26 nodes is 2048 MB, 2^24 cache is 576 MB.
        // On top of that, you will have the memory allocated by the JVM...

        System.out.println("Going to make two variables and compute their conjunction.");

        // We create BDDs that hold just "a" and "b" (i.e. x_1, x_2)
        long a = JSylvan.ref(JSylvan.makeVar(1));
        long b = JSylvan.ref(JSylvan.makeVar(2));

        // Create a BDD representing a /\ b
        long aAndB = JSylvan.ref(JSylvan.makeAnd(a, b));

        // Create a BDD set of variables 1,2,3,4,5
        long setOfVariables = JSylvan.ref(JSylvan.makeSet(new int[]{1,2,3,4,5}));

        System.out.println("Going to compute the number of satisfying assignments with 5 variables.");

        // Calculate the number of satisfying assignments, given domain of vars 1,2,3,4,5
        // This is... 8! 11000, 11001, 11010, 11011, 11100, 11101, 11110, 11111.
        double count = JSylvan.satcount(aAndB, setOfVariables);
        System.out.println(String.format("Number of satisfying assignments: %.0f (should be 8)", count));

        // Compute amount of nodes in A and b.
        long numberOfNodes = JSylvan.nodecount(aAndB);
        System.out.println(String.format("Number of nodes in the BDD: %d (should be 3)", numberOfNodes));

        System.out.println("Going to test existential quantification...");

        // Calculate \exists a * a /\ b
        // Obviously, the result should be "0 \/ b" = "b"
        long result = JSylvan.ref(JSylvan.makeExists(aAndB, a));
        if (result != b) System.out.println("Fail test 1.");

        long c = JSylvan.ref(JSylvan.makeVar(3));
        long d = JSylvan.ref(JSylvan.makeVar(4));
        long e = JSylvan.ref(JSylvan.makeVar(5));

        result = JSylvan.ref(JSylvan.makeUnionPar(new long[]{a, b, c, d, e}));
        if (result != JSylvan.makeOr(JSylvan.makeOr(a, b),JSylvan.makeOr(c,JSylvan.makeOr(d,e)))) System.out.println("Fail test 2.");

        // And that concludes our little demonstration. TODO: make proper test class...
        System.out.println("Simple tests success!");
    }
}
