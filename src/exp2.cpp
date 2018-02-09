#include <bits/stdc++.h>
#include <chrono>

using namespace std;
#define NCOLORS 10
#define INF     INT_MAX

// for debugging
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


bool check_satisfiability(int dist, string regex_op) {
    if (regex_op.size() == 0) {
        return (dist == 1);
    } else if (regex_op == "+") {
        return true;
    } else {
        // c<k type
        int k = 0;
        // start from third character since first 2 are '<='
        for (int i = 0; i < regex_op.size(); i++) {
            // extract `k` from string
            k = k * 10 + regex_op[i] - '0';
        }
        return (dist <= k);
    }
    return false;
}

void find_satisfying_nodes(
        int u,
        unordered_set<int> end_nodes,
        int* color_frequency,
        vector<char> regex_colors,
        vector<string> regex_ops,
        FILE* fp) {
    int len = regex_colors.size();
    unordered_set<int> begin_nodes;
    begin_nodes.insert(u);
    for (int color_index = 0; color_index < len; color_index++) {
        int color = regex_colors[color_index] - 'a';
        int N = color_frequency[color];
        int* color_table = new int[3 * N];
        find_color_pairs(color_table, color, color_frequency, fp);
        vector<int> candidate_next_begin;
        for (int i = 0; i < 3 * N; i += 3) {
            bool check = check_satisfiability(color_table[i + 2], regex_ops[color_index]);
            if (check) {
                if (begin_nodes.find(color_table[i]) != begin_nodes.end()) {
                    candidate_next_begin.push_back(color_table[i + 1]);
                }
            }
        }
        unordered_set<int> tmp(candidate_next_begin.begin(), candidate_next_begin.end());
        begin_nodes = tmp;
        delete [] color_table;
    }
    // check for common elements between begin_nodes & end_nodes
    unordered_set<int>::iterator it;
    for (it = begin_nodes.begin(); it != begin_nodes.end(); it++) {
        if (end_nodes.find(*it) != end_nodes.end()) {
            cout << "(" << u << ", " << *it << ")" << endl;
        }
    }
    return;
}


void evaluate_query(unordered_set<int> begin_nodes, unordered_set<int> end_nodes, int* color_frequency, string regex, FILE* fp) {
    vector<char> regex_colors;
    vector<string> regex_ops;
    split_regex(regex, regex_colors, regex_ops);
    unordered_set<int>::iterator it;
    for (it = begin_nodes.begin(); it != begin_nodes.end(); it++) {
        find_satisfying_nodes(*it, end_nodes, color_frequency, regex_colors, regex_ops, fp);
    }
    return;
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

    int*** BFS_mat = new int**[NCOLORS];
    for (int i = 0; i < NCOLORS; i++) {
        BFS_mat[i] = new int*[versize];
        for (int j = 0; j < versize; j++) {
            BFS_mat[i][j] = new int[versize];
            for (int k = 0; k < versize; k++) {
                BFS_mat[i][j][k] = INF;
            }
        }
    }

    BFS_matrix_build(adj, versize, BFS_mat);

    int* color_frequency = (int*)malloc(NCOLORS * sizeof(int));
    FILE* fp = fopen("../log/BFS_mat_1.txt", "r+");
    save_BFS_matrix(BFS_mat, versize, color_frequency, fp);

    for (int i = 0; i < NCOLORS; i++) {
        for (int j = 0; j < versize; j++) {
            delete [] BFS_mat[i][j];
        }
        delete [] BFS_mat[i];
    }
    delete [] BFS_mat;

	string uatt, vatt, regex;
	int querysize;
    int num_queries;

	cin >> querysize;
    cout << "Number of Queries: " << querysize << endl;
    vector<int> time_record;
	while (querysize--) {
        // perform query
		num_queries += 1;
        cout << "Starting Query " << num_queries << endl;
		cin >> uatt >> vatt >> regex;
        auto start = std::chrono::high_resolution_clock::now();
        vector<int> begin = find_candidate_nodes(uatt, att1, att2, att3, att4, versize);
        vector<int> end = find_candidate_nodes(vatt, att1, att2, att3, att4, versize);
        unordered_set<int> begin_nodes(begin.begin(), begin.end());
        unordered_set<int> end_nodes(end.begin(), end.end());
        evaluate_query(begin_nodes, end_nodes, color_frequency, regex, fp);
        auto diff = std::chrono::high_resolution_clock::now() - start;
        auto t1 = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
        cout << "Time Taken: " << t1.count() << endl;
    }
    return 0;
}
