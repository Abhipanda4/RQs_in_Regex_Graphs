#!/bin/bash

echo "Making binary files for each of the 3 methods..."
g++ ./src/trans_closure.cpp -o ./src/trans_closure
g++ ./src/partial_indexing_BFS.cpp -o ./src/partial_indexing_BFS
g++ ./src/BFS_efficient.cpp -o ./src/BFS_efficient

echo "Binary files successfully created!"

# remove previous dat
rm final_data.csv

#for N in {1000..30000..1000}
for N in {1000..1100..1000}
do
    echo "Simulating on a graph with $N vertices"
    M=$((5*$N))
    # Build the PaRMAT for crating edges
    ./PaRMAT/Release/PaRMAT -output tmp.txt -nVertices $N -nEdges $M -noEdgeToSelf -noDuplicateEdges
    cat tmp.txt | tr "\\t" "," > out.txt
    # build the graph with node predicates and edge colors and varying MC store size
    python data/graph_gen.py --inpfile=out.txt --nVertices=$N > temp_graph.txt
    # append the queries at the end of the graph file
    cat queries.txt >> temp_graph.txt
    # temp_graph.txt is the final graph to be fed into the algorithm
    cd src
    echo -e "\t-> Using transitive closure algorithm"
    ./trans_closure > data_algo1.csv
    echo -e "\t-> Using Partial Indexing and BFS algorithm"
    ./partial_indexing_BFS > data_algo2.csv
    echo -e "\t-> Using only BFS algorithm"
    ./BFS_efficient > data_algo3.csv
    echo "All algorithms ran successfully ..."
    echo "Saving data"
    #paste -d"," tmp1.csv tmp2.csv tmp3.csv >> ../final_data.csv
    cd ..
done
rm src/tmp*
