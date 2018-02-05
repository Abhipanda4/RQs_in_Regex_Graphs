#include <bits/stdc++.h>

using namespace std;
#define NCOLORS 10
#define INF     INT_MAX

void printArray(int* A, int N, int offset) {
    for (int i = 0; i < N; i += offset) {
        for (int j = 0; j < offset; j++) {
            cout << A[i + j] << " ";
        }
        cout << endl;
    }
}

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
	vector<string> attrib_ops;
	int i = 0;
	int len = s.size();
	while (i < len) {
		string temp;
		while (i < len && s[i] == '_') {
			attrib_ops.push_back("_");
			i++;
		}
		if (i == len) {
			break;
		}
		while (!isalnum(s[i])) {
			temp += s[i];
			i++;
		}
		attrib_ops.push_back(temp);
		temp = "";
		while (i < len && isalnum(s[i])) {
			temp += s[i];
			i++;
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
			while (!q.empty()) {
				pair<int, int> v = q.front();
				q.pop();
				if (v.first != curr_vertex) {
					// new vertex found! Store its distance
					M[col][curr_vertex][v.first] = v.second;
				}
                vector<int>::iterator it;
				for(it = adj[v.first][col].begin(); it != adj[v.first][col].end(); ++it) {
					if (!cyclefound && *it == curr_vertex) {
						// first self loop; shortest path from curr_vertex to itself found
						M[col][curr_vertex][curr_vertex] = v.second + 1;
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
    // *******************************************************************************
    // DEBUG
    // *******************************************************************************
    //cout << "Printing BFS matrix" << endl;
    //for (int c = 0; c < NCOLORS; c++) {
        //cout << c << endl;
        //for (int u = 0; u < versize; u++) {
            //for (int v = 0; v < versize; v++) {
                //cout << M[c][u][v] << " ";
            //}
            //cout << endl;
        //}
    //}
    // *******************************************************************************
    // *******************************************************************************
	return;
}


void save_BFS_matrix(int*** BFS_mat, int versize, int* color_frequency, FILE* fp) {
    for (int c = 0; c < NCOLORS; c++) {
        int count = 0;
        for (int u = 0; u < versize; u++) {
            for (int v = 0; v < versize; v++) {
                // *******************************************************************************
                // DEBUG
                // *******************************************************************************
                //cout << u << " - " << v << endl;
                //cout << BFS_mat[c][u][v] << endl;
                // *******************************************************************************
                // *******************************************************************************
                if (BFS_mat[c][u][v] != INF) {
                    // write (u, v, d) into the file
                    count += 1;
                    fwrite(&u, sizeof(int), 1, fp);
                    fwrite(&v, sizeof(int), 1, fp);
                    fwrite(&BFS_mat[c][u][v], sizeof(int), 1, fp);
                }
            }
        }
        color_frequency[c] = count;
    }
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


void find_color_pairs(int* color_table, int color, int* color_frequency, FILE* fp) {
    int N = 3 * color_frequency[color];
    int offset = 0;
    for (int i = 0; i < color; i++) {
        offset += color_frequency[i];
    }
    offset *= 3;
    fseek(fp, offset * sizeof(int), SEEK_SET);
    fread(color_table, sizeof(int), N, fp);
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
    int index_of_min_freq_color = find_color_with_min_frequency(regex_colors, color_frequency);
    int color = regex_colors[index_of_min_freq_color] - 'a';

    // extract the data for `color`
    int N = color_frequency[color];
    int* color_table = (int*)malloc(sizeof(int) * 3 * N);
    find_color_pairs(color_table, color, color_frequency, fp);
    //printArray(color_table, 3 * N, 3);

    // filter all pairs of vertices satisfying the
    // constraint due to corresponding op symbol
    vector<int> candidate_begin;
    vector<int> candidate_end;
    for (int i = 0; i < 3 * N; i += 3) {
        bool check = check_satisfiability(color_table[i], color_table[i + 1], color_table[i + 2], regex_ops[index_of_min_freq_color]);
        if (check) {
            candidate_begin.push_back(color_table[i]);
            candidate_end.push_back(color_table[i + 1]);
        }
    }
    if (candidate_begin.size() == 0)
        return false;

    int len = regex_colors.size();
    // *******************************************************************************
    // DEBUG
    // *******************************************************************************
    //cout << "Regex is of length: " << len << endl;
    //cout << "Index of min freq color: " << index_of_min_freq_color << endl;
    // *******************************************************************************
    // *******************************************************************************

    if (len == 1) {
        int L = candidate_begin.size();
        unordered_set<int>::iterator it;
        unordered_set<int> next_begin_nodes;
        for (int i = 0; i < L; i++) {
            if (begin_nodes.find(candidate_begin[i]) != begin_nodes.end()) {
                next_begin_nodes.insert(candidate_end[i]);
            }
        }
        L = candidate_end.size();
        unordered_set<int> next_end_nodes;
        for (int i = 0; i < L; i++) {
            if (end_nodes.find(candidate_end[i]) != end_nodes.end()) {
                next_end_nodes.insert(candidate_begin[i]);
            }
        }
        return (next_begin_nodes.size() != 0) && (next_end_nodes.size() != 0);
    } else if (index_of_min_freq_color == 0 && len >= 2) {
        // check for any intersection betweeen `candidate_begin` nodes,
        // and `begin_nodes`
        int L = candidate_begin.size();
        unordered_set<int> next_begin_nodes;
        for (int i = 0; i < L; i++) {
            if (begin_nodes.find(candidate_begin[i]) != begin_nodes.end()) {
                next_begin_nodes.insert(candidate_end[i]);
            }
        }
        vector<char> new_regex_colors(regex_colors.begin() + 1, regex_colors.end());
        vector<string> new_regex_ops(regex_ops.begin() + 1, regex_ops.end());
        return (next_begin_nodes.size() != 0) &&
                recursive_search(next_begin_nodes, end_nodes, color_frequency, new_regex_colors, new_regex_ops, fp);
    } else if (index_of_min_freq_color == len - 1 && len >= 2) {
        // check for any intersection between `candidate_end` nodes and
        // `end_nodes`
        int L = candidate_end.size();
        unordered_set<int> next_end_nodes;
        for (int i = 0; i < L; i++) {
            if (end_nodes.find(candidate_end[i]) != end_nodes.end()) {
                next_end_nodes.insert(candidate_begin[i]);
            }
        }
        vector<char> new_regex_colors(regex_colors.begin(), regex_colors.end() - 1);
        vector<string> new_regex_ops(regex_ops.begin(), regex_ops.end() - 1);
        return (next_end_nodes.size() != 0) &&
                recursive_search(begin_nodes, next_end_nodes, color_frequency, new_regex_colors, new_regex_ops, fp);
    } else {
        // call recursively
        vector<char> begin_regex_color(&regex_colors[0], &regex_colors[index_of_min_freq_color]);
        vector<string> begin_regex_ops(&regex_ops[0], &regex_ops[index_of_min_freq_color]);
        vector<char> end_regex_color(regex_colors.begin() + index_of_min_freq_color + 1, regex_colors.end());
        vector<string> end_regex_ops(regex_ops.begin() + index_of_min_freq_color + 1, regex_ops.end());
        // *******************************************************************************
        // DEBUG
        // *******************************************************************************
        //cout << "Mark1" << endl;
        //cout << begin_regex_color.size() << endl;
        //cout << begin_regex_color[0] << endl;
        //cout << "Mark2" << endl;
        // *******************************************************************************
        // *******************************************************************************

        unordered_set<int> next_end_nodes(candidate_begin.begin(), candidate_begin.end());
        unordered_set<int> next_begin_nodes(candidate_end.begin(), candidate_end.end());
        return recursive_search(begin_nodes, next_end_nodes, color_frequency, begin_regex_color, begin_regex_ops, fp) &&
               recursive_search(next_begin_nodes, end_nodes, color_frequency, end_regex_color, end_regex_ops, fp);
    }
}


bool evaluate_query(unordered_set<int> begin_nodes, unordered_set<int> end_nodes, int* color_frequency, string regex, FILE* fp) {
    vector<char> regex_colors;
    vector<string> regex_ops;
    split_regex(regex, regex_colors, regex_ops);
    bool result = recursive_search(begin_nodes, end_nodes, color_frequency, regex_colors, regex_ops, fp);
    return result;
}


int main() {
    freopen("../data/graph.txt", "r", stdin);
	int versize, edgesize;

	cin >> versize >> edgesize;
	vector<int> att1(versize); // Unique ID
	vector<string> att2(versize); // Name
	vector<string> att3(versize); // Country
	vector<string> att4(versize); // Sex
	for (int i = 0; i < versize; i++) {
		cin >> att1[i] >> att2[i] >> att3[i] >> att4[i];
	}

	vector< vector< vector<int> > > adj(versize, vector< vector<int> >(NCOLORS));
	int u, v;
	char color;
	for (int i = 0; i < edgesize; i++){
		cin >> u >> v >> color;
		adj[u - 1][color - 'a'].push_back(v - 1);
	}

    int*** BFS_mat = (int***)malloc(NCOLORS * sizeof(int**));
    for (int i = 0; i < NCOLORS; i++) {
        BFS_mat[i] = (int**)malloc(versize * sizeof(int*));
        for (int j = 0; j < versize; j++) {
            BFS_mat[i][j] = (int*)malloc(versize * sizeof(int));
            for (int k = 0; k < versize; k++) {
                BFS_mat[i][j][k] = INF;
            }
        }
    }

    BFS_matrix_build(adj, versize, BFS_mat);

    int* color_frequency = (int*)malloc(NCOLORS * sizeof(int));
    FILE* fp = fopen("../log/BFS_mat_1.txt", "r+");
    save_BFS_matrix(BFS_mat, versize, color_frequency, fp);

	string uatt, vatt, regex;
	int querysize;
    double t;
    double total_t = 0;
    int num_queries;

	cin >> querysize;
    cout << "Number of Queries: " << querysize << endl;
    vector<int> time_record;
	while (querysize--) {
        // perform query
		num_queries += 1;
		cin >> uatt >> vatt >> regex;
		clock_t t;
		t = clock();
        vector<int> begin = find_candidate_nodes(uatt, att1, att2, att3, att4, versize);
        vector<int> end = find_candidate_nodes(vatt, att1, att2, att3, att4, versize);
        unordered_set<int> begin_nodes(begin.begin(), begin.end());
        unordered_set<int> end_nodes(end.begin(), end.end());
        bool check = evaluate_query(begin_nodes, end_nodes, color_frequency, regex, fp);
        if (check)
            cout << "RQ successful !!..." << endl;
        else
            cout << "Better luck next time !!...." << endl;
        t = clock() - t;
        t = (double)t / CLOCKS_PER_SEC;
        time_record.push_back(t);
        total_t += t;
    }
    cout << endl << "Time taken: " << total_t << endl;
    return 0;
}
