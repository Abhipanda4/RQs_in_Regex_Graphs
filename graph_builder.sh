echo "Building graphs ..."
nodes=(100000 200000 500000 1000000)
for N in "${nodes[@]}"
do
    M=$((3 * $N))
    echo "Building graphs with $N nodes"
    # Build the PaRMAT for crating edges
    ./PaRMAT/Release/PaRMAT -output data_files/tmp.txt -nVertices $N -nEdges $M -noEdgeToSelf -noDuplicateEdges
    cat data_files/tmp.txt | tr "\\t" "," > data_files/out.txt
    # build the graph with node predicates and edge colors and varying MC store size
    python generators/graph_gen.py --inpfile=data_files/out.txt --nVertices=$N > data_files/graph_$N.txt
    # append the queries at the end of the graph file
    python generators/query_gen.py --Q=20 > data_files/queries.txt
    cat data_files/queries.txt >> data_files/graph_$N.txt
done

rm data_files/tmp*
rm data_files/out*
