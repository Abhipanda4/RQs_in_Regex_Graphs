#!/bin/bash

# first obtain an edge file using PaRMAT
./PaRMAT/Release/PaRMAT -output $1 -nVertices $2 -nEdges $3 -noEdgeToSelf -noDuplicateEdges
cat $1 | tr "\\t" "," > out.txt

# run the python script
python graph_gen.py --inpfile=out.txt --nVertices=$2 --nColors=$4 > sim.txt
rm out.txt
