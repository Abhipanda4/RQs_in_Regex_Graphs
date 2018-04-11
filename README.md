## Regular Path Queries Using Monte Carlo Framework
This work tries to optimize regular expression queries based on a data graph by calculating a partial index using Monte carlo sampling.

A data graph is a collection of nodes and edges. For this work, the synthetic graphs considered have the following characterstics:
1. Each node is represented by a set of 6 attrbutes:
    * Year of birth(integer)
    * Name(string)
    * Gender(string)
    * Number of friends(integer)
    * Number of posts(integer)
    * UID(integer)

2. The label for any edge is a letter like 'a', 'b', etc.

The queries are specified by using node predicates and regular expressions:
1. A node predicate can be specified by an ops string like:
    * \___\_==F<=81<=158_\_
    * The possible ops for strings are: == & !=
    * The possible ops for integers are: ==, >, >=, <, <=, !=
    * In case we don't care about some attribute, it is specified by __(double underscore)
2. The regular expression is a string formed from the grammar S -> c | c<=k | c+; where c is a color and k is any integer.

## Instructions on executing and preparing data graphs

1. Go into the src folder. There are 3 cpp files
    * partial\_indexing_BFS.cpp : This is responsible for running monte-carlo simulations to calculate a partial index and then answer Regular path Queries using this index.
    * trans\_closure.cpp : This calculates a full transitive closure for the data graph and uses it to answer the queries.
    * BFS\_efficient.cpp : This implemenatation uses simple BFS algorithm to find possible paths.

2. To compile any source file, use command:
    ```
    g++ -std=c++11 <source-file-name>
    ```
    Optionally, you can use a -o flag with g++ to give the binary a custom name
    ```
    g++ -std=c++11 <source-file-name> -o <custom-name>
    ```

3. To finally run the program, use:
    ```
    ./a.out <target-graph-file>
    ```
    where __target-graph-file__ is the graph file on which queries are to be performed. A sample file has been provided in data_files folder, named graph_15000.txt which has 15000 nodes and 45000 edges.
