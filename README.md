# About
This repository contains C++ code for generating graphs using various graph models and
calculating their Weiner index (sum of shortest distances between every pair of vertices).

Implemented as a student project for Graph Modelling of Systems (pol. *Grafowe Modelowanie Systemów*)

# Running
Compile using g++ or equivalent (see `compile.sh`)

When run, the program expects an ID of the graph model and model parameters (one or two, depending
on the model) through STDIN and will print the calculated Weiner index to STDOUT.

If program is run with argument `p` (`./gms p`), it will instead print a list of all edges of the
generated graph, as two vertex indices separated by space with each pair separated by a newline.
For example, a full graph with 4 vertices would print:
```
0 1
0 2
0 3
1 2
1 3
2 3
```

Example inputs and outputs are found in `tests` folder. Name of each file in that folder indicates
the model index, and file extension indicates if it is an input (`.in`) or expected output (`.out`).
This program was tested for those parameters. Bigger graphs may result in very long execution time
and/or high memory usage.
