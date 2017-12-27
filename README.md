JSylvan
=======
JSylvan allows using [Sylvan](https://github.com/utwente-fmt/sylvan) from Java using JNI.

Sylvan is a parallel (multi-core) BDD library in C. Sylvan allows both sequential and parallel BDD-based algorithms to benefit from parallelism. Sylvan uses the work-stealing framework Lace and a scalable lockless hashtable to implement scalable multi-core BDD operations.

JSylvan and Sylvan are developed (&copy; 2011-2014) by the [Formal Methods and Tools](http://fmt.ewi.utwente.nl/) group at the University of Twente as part of the MaDriD project, which is funded by NWO. JSylvan is licensed with the Apache 2.0 license.

See also: https://github.com/utwente-fmt/sylvan

Compiling
---------
Requires jni.h in the path for Maven to find.

Usage
-----
You can find a simple example in `java/src/main/java/jsylvan/JSylvanDemo.java`.

Updating to the latest version of Sylvan
----------------------------------------
Use the following command:
```
git subtree pull --prefix=native/sylvan git@github.com:utwente-fmt/sylvan master --squash -m "Update Sylvan to latest version"
```

