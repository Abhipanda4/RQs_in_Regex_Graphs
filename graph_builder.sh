nodes=(10000 12000 14000 16000 18000 20000)
if [ -z "$1" ]
then
    echo "Generating 50 queries by default"
    queries=50
else
    queries=$1
fi
for N in "${nodes[@]}"
do
    M=$((3 * $N))
    echo "Building graphs with $N nodes and $M edges"
    # Build the PaRMAT for crating edges
    ./PaRMAT/Release/PaRMAT -output data_files/tmp.txt -nVertices $N -nEdges $M -noEdgeToSelf -noDuplicateEdges
    cat data_files/tmp.txt | tr "\\t" "," > data_files/out.txt
    # build the graph with node predicates and edge colors and varying MC store size
    python generators/graph_gen.py --inpfile=data_files/out.txt --nVertices=$N > data_files/graph_$N.txt
    # append the queries at the end of the graph file
    python generators/query_gen.py --Q=$queries > data_files/queries.txt
    cat data_files/queries.txt >> data_files/graph_$N.txt
done

rm data_files/tmp*
rm data_files/out*
