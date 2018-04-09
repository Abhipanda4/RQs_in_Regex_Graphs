## Regular Path Queries Using Monte Carlo Framework
This work tries to optimize regular expression queries based on a data graph by calculating a partial index based on Monte carlo sampling.

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
