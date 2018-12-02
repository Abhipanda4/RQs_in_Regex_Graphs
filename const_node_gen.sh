edges=(2 3 4 5 6)
if [ -z "$1" ]
then
    echo "Generating 50 queries by default"
    queries=50
else
    queries=$1
fi
python generators/query_gen.py --Q=$queries > data_files/queries.txt
N=15000
for E in "${edges[@]}"
do
    M=$((15000 * $E))
    echo "Building graphs with 15000 nodes and $M edges"
    # Build the PaRMAT for crating edges
    ./PaRMAT/Release/PaRMAT -output data_files/tmp.txt -nVertices $N -nEdges $M -noEdgeToSelf -noDuplicateEdges
    cat data_files/tmp.txt | tr "\\t" "," > data_files/out.txt
    # build the graph with node predicates and edge colors and varying MC store size
    python generators/graph_gen.py --inpfile=data_files/out.txt --nVertices=$N > data_files/graph_15000_$E.txt
    # append the queries at the end of the graph file
    cat data_files/queries.txt >> data_files/graph_15000_$E.txt
done

rm data_files/tmp*
rm data_files/out*
