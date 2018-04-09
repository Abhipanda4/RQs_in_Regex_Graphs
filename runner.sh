#!/bin/bash

echo "Making binary files for each of the 3 methods..."
g++ -std=c++11 ./src/trans_closure.cpp -o ./src/trans_closure
g++ -std=c++11 ./src/partial_indexing_BFS.cpp -o ./src/partial_indexing_BFS
g++ -std=c++11 ./src/BFS_efficient.cpp -o ./src/BFS_efficient
echo "Binary files successfully created!"

if [ -a "data_files/data_algo2.csv" ]
then
    echo "Removing old data files"
    rm data_files/data_algo*
    rm data_files/index_sizes
fi

echo "Starting analysis ..."
cd src
for N in {10000..20000..2000}
do
    echo ""
    echo "Simulating on a graph with $N vertices"
    echo -e "\t-> Using transitive closure algorithm"
    ./trans_closure ../data_files/graph_$N.txt >> ../data_files/data_algo1.csv
    du -h ../BFS_index/trans_closure >> ../data_files/index_sizes
    echo -e "\t-> Using Partial Indexing and BFS algorithm"
    ./partial_indexing_BFS ../data_files/graph_$N.txt >> ../data_files/data_algo2.csv
    echo -e "\t-> Using only BFS algorithm"
    ./BFS_efficient ../data_files/graph_$N.txt >> ../data_files/data_algo3.csv
done

echo "Cleaning up ..."
rm ./src/trans_closure
rm ./src/partial_indexing_BFS
rm ./src/BFS_efficient
