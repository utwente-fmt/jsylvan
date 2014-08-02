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

class MCFile
{
    private long vectorSize;
    private long bitsPerInteger;

    private long initial;
    private long domain;

    private long[] relations;
    private long[] relation_domains;

    private native static MCFile fromFile(String filename);

    public long bfs() {
        long level_counter = 1;
        long states = initial;
        long new_states = JSylvan.ref(states);
        do {
            System.out.format("Level %d\n", level_counter++);
            // System.out.format("Level %d: %.0f states\n", level_counter++, JSylvan.satcount(states, domain)); // to do variables
            long cur_states = new_states;
            new_states = JSylvan.getFalse();
            for (int i=0; i<relations.length; i++) {
                long a = JSylvan.ref(JSylvan.makeNext(cur_states, relations[i], relation_domains[i]));
                long b = JSylvan.ref(JSylvan.makeIte(states, JSylvan.getFalse(), a)); // b = a - states
                JSylvan.deref(a);
                long c = JSylvan.ref(JSylvan.makeOr(new_states, b));
                JSylvan.deref(b);
                JSylvan.deref(new_states);
                new_states = c;
            }
            JSylvan.deref(cur_states);
            long temp = JSylvan.ref(JSylvan.makeOr(states, new_states));
            JSylvan.deref(states);
            states = temp;
        } while (new_states != JSylvan.getFalse());
        JSylvan.deref(new_states);

        return states;
    }

    public static void main(String[] args) {
        if (args.length == 0) {
            System.out.println("Use: jsylvan.MCFile <filename> [workers]");
            return;
        }

        if (args.length == 2) JSylvan.initialize(Integer.parseInt(args[1]), 100000, 30, 26, 4);
        else JSylvan.initialize(4, 100000, 30, 26, 4);

        MCFile f = MCFile.fromFile(args[0]);

        System.out.format("Read '%s': state vector: %d ints, %d bits per int, %d transition groups\n", args[0], f.vectorSize, f.bitsPerInteger, f.relations.length);
        System.out.format("Initial states: %d BDD nodes\n", JSylvan.nodecount(f.initial));
        for (int i=0; i<f.relations.length; i++) System.out.format("Transition %d: %d BDD nodes\n", i, JSylvan.nodecount(f.relations[i]));

        final long t1 = System.nanoTime();
        long result = f.bfs();
        final long t2 = System.nanoTime();

        System.out.format("Expired time: %d.%d seconds\n", (t2-t1)/1000000000, (t2-t1)%1000000000);

        System.out.format("Final result: %.0f states\n", JSylvan.satcount(result, f.domain));
        System.out.format("Final result: %d nodes\n", JSylvan.nodecount(result));
    }
}
