//#include <vector>
//#include <set>
//#include <queue>
//#include <sstream>
//#include <iostream>
//#include <fstream>
//#include <cstring>
//#include <ctime>
//#include <cmath>
//#include <limits.h>
#include <bits/stdc++.h>

using namespace std;
#define NCOLORS 10
#define INF     INT_MAX


bool matchstr(string att, string op, string val) {
    /*
     * This function finds whether the node attribute
     * string `att` is satisfied by string `val` and
     * string operation `op`
     */
	if (val == "_") {
		return true;
	}
	else if (op == "==") {
		return val == att ? true : false;
	}
	else {
		return val != att ? true : false;
	}
}


bool matchnum(int att, string op, string val) {
    /*
     * This function finds whether the node attribute
     * string `att` is satisfied by integer `val`
     * and string operation `op`
     */
	if (val == "_") {
		return true;
	}
	else if (op == "<=") {
		return att <= stoi(val)? true: false;
	}
	else if (op == "<") {
		return att < stoi(val)? true: false;
	}
	else if (op == ">=") {
		return att >= stoi(val)? true: false;
	}
	else if (op == ">") {
		return att > stoi(val)? true: false;
	}
	else if (op == "==") {
		return att == stoi(val)? true: false;
	}
	else if (op == "!=") {
		return att != stoi(val)? true: false;
	}
}


void split_regex(string s, vector<char> &regex_colors, vector<string> &regex_ops) {
    /*
     * Given a regular expression string `s`;
     * this function splits it into individual
     * unicolor components.
     */
	int len = s.size();

	for (int i = 0; i < len;) {
		int j = i + 1;
		while (j < len && !isalpha(s[j])) {
			// continue till an alphabet is encountered
			j++;
		}
		// j denotes the position of next color symbol
        regex_colors.push_back(s[i]);
        i += 1;
		regex_ops.push_back(s.substr(i, j-i));
		i = j;
	}
	return;
}


vector<string> find_attribs(string s) {
    /*
     * Given a string of the form [op][val][op][val]...
     * for the node properties, this function extracts
     * the ops and attribute values into a string vector
     */
	int i = 0;
	int len = s.size();
    string temp = "";
    vector<string> attrib_ops;
	while (i < len) {
        if (s[i] == '_') {
            attrib_ops.push_back("_");
            i += 2;
        }
        if (i == len) break;
        temp = "";
        while (!isalnum(s[i])) {
            temp += s[i];
            i += 1;
        }
        attrib_ops.push_back(temp);
        temp = "";
        while (i < len && isalnum(s[i])) {
            temp += s[i];
            i += 1;
        }
        attrib_ops.push_back(temp);
    }
    return attrib_ops;
}


vector <int> find_candidate_nodes(
		string matstr,
		vector<int> &att1,
		vector<string> &att2,
		vector<string> &att3,
		vector<string> &att4,
		int versize
		) {

	vector<string> attrib_ops = find_attribs(matstr);
	vector<int> match_u;

	for (int i = 0; i < versize; i++) {
		if (matchnum(att1[i], attrib_ops[0], attrib_ops[1])
			&& matchstr(att2[i], attrib_ops[2], attrib_ops[3])
			&& matchstr(att3[i], attrib_ops[4], attrib_ops[5])
			&& matchstr(att4[i], attrib_ops[6], attrib_ops[7])) {
			// node i staisfies all attribute constraints
			// so it is an eligible candidate for the query
			match_u.push_back(i);
		}
	}
	return match_u;
}


void BFS_matrix_build(vector< vector< vector<int> > > &adj, int versize, int*** M) {
	for (int i = 0; i < versize; i++) {
		for (int col = 0; col < NCOLORS; col++) {
			int curr_vertex = i;
			// Perform BFS using a queue of (node, distance) pairs
			queue< pair<int,int> > q;
			q.push(make_pair(curr_vertex, 0));
			vector<bool> flag(versize, 0);
			bool cyclefound = false;
			while(!q.empty()){
				pair<int, int> v = q.front();
				q.pop();
				if (v.first != curr_vertex) {
					// new vertex found! Store its distance
					M[curr_vertex][v.first][col] = v.second;
				}
                vector<int>::iterator it;
				for(it = adj[v.first][col].begin(); it != adj[v.first][col].end(); ++it) {
					if (!cyclefound && *it == curr_vertex) {
						// first self loop; shortest path from curr_vertex to itself found
						M[curr_vertex][curr_vertex][col] = v.second + 1;
						cyclefound = true;
						continue;
					}
					if (!flag[*it]) {
						// push all unvisited vertices connected to the node
						// with current color into the queue
						// Also, distance is increased by 1
						q.push(make_pair(*it, v.second + 1));
						// mark the node visited
						flag[*it] = true;
					}
				}
			}
		}
	}
	return;
}


void save_BFS_matrix(int*** BFS_mat, int versize, int* color_frequency) {
    FILE* fp = fopen("../log/BFS_mat_1.txt", "w");
    for (int c = 0; c < NCOLORS; c++) {
        int count = 0;
        for (int u = 0; u < versize; u++) {
            for (int v = 0; v < versize; v++) {
                if (BFS_mat[u][v][c] != INF) {
                    // write (u, v, d) into the file
                    count += 1;
                    fwrite(&u, sizeof(int), 1, fp);
                    fwrite(&v, sizeof(int), 1, fp);
                    fwrite(&BFS_mat[u][v][c], sizeof(int), 1, fp);
                }
            }
        }
        color_frequency[c] = count;
    }
    fclose(fp);
    return;
}

int find_color_with_min_frequency(vector<char> regex_colors, int* color_frequency) {
    /*
     * Given a list of color symbols and their frequency in the graph, return the
     * index of color symbol which has the minimum frequency
     * Index is required to identify the op symbol corresponding to it
     */
    int len = regex_colors.size();
    int min_index = 0;
    int min_till_now = INT_MAX;
    for (int i = 0; i < len; i++) {
        char color = regex_colors[i];
        int candidate = color_frequency[color - 'a'];
        if (candidate < min_till_now) {
            min_till_now = candidate;
            min_index = i;
        }
    }
    return min_index;
}


int* find_color_pairs(int color, int* color_frequency, FILE* fp) {
    int* result;
    int N = 3 * color_frequency[color];
    int offset = 0;
    for (int i = 0; i < color; i++) {
        offset += color_frequency[i];
    }
    offset *= 3;
    fseek(fp, offset * sizeof(int), SEEK_SET);
    fread(result, sizeof(int), N, fp);
    return result;
}


bool check_satisfiability(int u, int c, int dist, string regex_op) {
    if (regex_op.size() == 0) {
        // empty string
        return true;
    } else if (regex_op == "+") {
        return (dist >= 1);
    } else {
        // c<k type
        int k = 0;
        // start from third character since first 2 are '<='
        for (int i = 2; i < regex_op.size(); i++) {
            // extract `k` from string
            k = k * 10 + regex_op[i] - '0';
        }
        return (dist < k);
    }
    return false;
}


bool recursive_search(
        unordered_set<int> &begin_nodes,
        unordered_set<int> &end_nodes,
        int* color_frequency,
        vector<char> &regex_colors,
        vector<string> &regex_ops,
        FILE* fp) {
    /*
     * This function recursively evaluates the query
     */
    int len = regex_colors.size();
    // find color symbol with minimum frequency in the graph
    int index_of_min_freq_color = find_color_with_min_frequency(regex_colors, color_frequency);
    int color = regex_colors[index_of_min_freq_color] - 'a';

    // extract the data for `color`
    int* color_table = find_color_pairs(color, color_frequency, fp);

    // filter all pairs of vertices satisfying the
    // constraint due to corresponding op symbol
    vector<int> candidate_begin;
    vector<int> candidate_end;
    int N = color_frequency[color];
    for (int i = 0; i < N; i += 3) {
        bool check = check_satisfiability(color_table[i], color_table[i + 1], color_table[i + 2], regex_ops[index_of_min_freq_color]);
        if (check) {
            candidate_begin.push_back(color_table[i + 1]);
            candidate_end.push_back(color_table[i + 2]);
        }
    }
    if (candidate_begin.size() == 0)
        return false;

    if (index_of_min_freq_color == 0) {
        // check for any intersection betweeen `candidate_begin` nodes,
        // and `begin_nodes`
        int L = candidate_begin.size();
        unordered_set<int> next_begin_nodes;
        for (int i = 0; i < L; i++) {
            if (begin_nodes.find(candidate_begin[i]) != begin_nodes.end()) {
                next_begin_nodes.insert(candidate_end[i]);
            }
        }
        vector<char> new_regex_colors(&regex_colors[1], &regex_colors[len - 1]);
        vector<string> new_regex_ops(&regex_ops[1], &regex_ops[len - 1]);
        return (next_begin_nodes.size() != 0) &&
                recursive_search(next_begin_nodes, end_nodes, color_frequency, new_regex_colors, new_regex_ops, fp);
    } else if (index_of_min_freq_color == len - 1) {
        // check for any intersection between `candidate_end` nodes and
        // `end_nodes`
        int L = candidate_end.size();
        unordered_set<int> next_end_nodes;
        for (int i = 0; i < L; i++) {
            if (end_nodes.find(candidate_end[i]) != end_nodes.end()) {
                next_end_nodes.insert(candidate_end[i]);
            }
        }
        vector<char> new_regex_colors(&regex_colors[0], &regex_colors[len - 2]);
        vector<string> new_regex_ops(&regex_ops[0], &regex_ops[len - 1]);
        return (next_end_nodes.size() != 0) &&
                recursive_search(begin_nodes, next_end_nodes, color_frequency, new_regex_colors, new_regex_ops, fp);
    } else {
        // call recursively
        vector<char> begin_regex_color(&regex_colors[0], &regex_colors[index_of_min_freq_color - 1]);
        vector<string> begin_regex_ops(&regex_ops[0], &regex_ops[index_of_min_freq_color - 1]);
        vector<char> end_regex_color(&regex_colors[index_of_min_freq_color + 1], &regex_colors[len - 1]);
        vector<string> end_regex_ops(&regex_ops[index_of_min_freq_color + 1], &regex_ops[len - 1]);

        unordered_set<int> next_end_nodes(candidate_begin.begin(), candidate_begin.end());
        unordered_set<int> next_begin_nodes(candidate_end.begin(), candidate_end.end());
        return recursive_search(begin_nodes, next_end_nodes, color_frequency, begin_regex_color, begin_regex_ops, fp) &&
               recursive_search(next_begin_nodes, end_nodes, color_frequency, end_regex_color, end_regex_ops, fp);
    }
}

bool evaluate_query(unordered_set<int> begin_nodes, unordered_set<int> end_nodes, int* color_frequency, string regex) {
    vector<char> regex_colors;
    vector<string> regex_ops;
    split_regex(regex, regex_colors, regex_ops);
    FILE* fp = NULL;
    bool result = recursive_search(begin_nodes, end_nodes, color_frequency, regex_colors, regex_ops, fp);
    return result;
}

int main() {
    return 0;
}
