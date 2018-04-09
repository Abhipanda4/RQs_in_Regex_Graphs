#include <vector>
#include <set>
#include <queue>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cmath>
#include <limits.h>

using namespace std;

#define NCOLORS 10
#define INF     INT_MAX

bool matchstr(string att, string op, string val) {
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

vector<string> find_attribs(string s) {
    /*
        Given a string representing required attributes of all nodes;
        split the string into attributes and ops(<=, <, >, >=, ==, !=)
        for each vertex
    */
	vector<string> attrib_ops;
	int i = 0;
	int len = s.size();
	while (i < len) {
		string temp;
		// '_' denotes that any attribute would suffice
		// Hence no need to search for vertex
		while (i < len && s[i] == '_') {
			attrib_ops.push_back("_");
			i++;
		}
		if (i == len) {
			break;
		}
		// if character is not alpha-numeric, implies that it must be
		// an ops symbol. Concatenate all such non alpha-numeric strs
		// to form the complete op
		while (!isalnum(s[i])) {
			temp += s[i];
			i++;
		}
		// temp is now an ops string
		attrib_ops.push_back(temp);
		temp = "";
		while (i < len && isalnum(s[i])) {
			temp += s[i];
			i++;
		}
		// temp is now the value  of attribute that has to be satisfied
		attrib_ops.push_back(temp);
	}
	return attrib_ops;
}

vector <int> find_candidate_nodes(
		string matstr,
		int* att1,
		string* att2,
		string* att3,
		string* att4,
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

bool type1(int v, string q, int* M, int versize) {
	// corresponding to regex "c"(exactly 1 colored edge)
    int index = (q[0] - 'a') * versize + v;
    if (M[index] == 1) {
		return 1;
	}
	else {
		return 0;
	}
}


bool type2(int v, string q, int* M, int versize) {
	// corresponding to regex "c <= k"
	int k = 0;
	int len = q.size();
	for (int i = 1; i < len; i++) {
		k = k * 10 + q[i] - '0';
	}
    int index = (q[0] - 'a') * versize + v;
	if (M[index] <= k) {
		return 1;
	}
	else {
		return 0;
	}
}

bool type3(int v, string q, int* M, int versize) {
	// corresponding to regex "c+"
    int index = (q[0] - 'a') * versize + v;
	if (M[index] < INF && M[index] != 0) {
		return 1;
	}
	else {
		return 0;
	}

}

vector<string> split_query_string(string s) {
    /*
        Returns a vector of strings. Each string has 1st
        character as a color symbol. The characters following it
        define the type of regex.
    */
	vector<string> qs;
	int len = s.size();

	for (int i = 0; i < len;) {
		int j = i + 1;
		while (j < len && !isalpha(s[j])) {
			// continue till an alphabet is encountered
			j++;
		}
		// j denotes the position of next color symbol
		qs.push_back(s.substr(i, j-i));
		i = j;
	}
	return qs;
}

bool query_result(int* req_nodes, int u, int v, vector<string> qs, int versize, FILE* fp) {
    /*
        Checks whether nodes `u` and `v` are connected by a path satisfying
        the regex defined by qs
    */

    vector<bool> curr_set(versize, 0);
    vector<bool> next_set(versize, 0);
    curr_set[u] = 1;
    int len = qs.size();

    int num_nodes = NCOLORS * versize;
    for (int i = 0; i < len; i++) {
        int qsize = qs[i].size();
        for (int j = 0; j < versize; j++) {
            // read the corresponding part into memory
            fseek(fp, j * num_nodes * sizeof(int), SEEK_SET);
            fread(req_nodes, sizeof(int), num_nodes, fp);
            if (curr_set[j] != 0) {
                if(qsize == 1) {
                    // type 1 regex
                    for (int k = 0; k < versize; k++) {
                        if (type1(k, qs[i], req_nodes, versize)) {
                            next_set[k] = 1;
                        }
                    }
                }
                else if(qs[i][1] != '+') {
                    // type 2 regex
                    for (int k = 0; k < versize; k++) {
                        if (type2(k, qs[i], req_nodes, versize)) {
                            next_set[k] = 1;
                        }
                    }
                }
                else {
                    // type 3 regex
                    for (int k = 0; k < versize; k++) {
                        if (type3(k, qs[i], req_nodes, versize)) {
                            next_set[k] = 1;
                        }
                    }
                }
            }
        }
        // consider pseudo nodes for next pass
        curr_set = next_set;
        fill(next_set.begin(), next_set.end(), 0);
    }
    return curr_set[v] == 1 ? true : false;
}


void build_BFS_matrix(int* BFS_mat, int versize, vector< vector< vector<int> > > &adjacency_mat) {
    /*
       Build a BFS matrix using queue of (vertex, distance) pairs
       The BFS matrix would store the distance between 2 vertices;
       where the path must consist of only 1 color.

       If no unicolor path is found, the distnce is infinite.
   */
    int size = versize * versize * NCOLORS;
    for (int i = 0; i < size; i++)
        BFS_mat[i] = INF;

    int index, row_term;
	for (int curr_vertex = 0; curr_vertex < versize; curr_vertex++) {
        row_term = curr_vertex * NCOLORS * versize;
        // this for loop iterates over each color;
        // since we require only unicolor paths
		for (int color = 0; color < NCOLORS; color++) {
			queue< pair<int,int> > q;
            // push the origin of BFS tree with distance = 0
			q.push(make_pair(curr_vertex, 0));
			vector<bool> flag(versize, 0);
			bool cyclefound = false;
			while(!q.empty()) {
				pair<int, int> v = q.front();
				q.pop();
				if (v.first != curr_vertex) {
					// new vertex found! Store its distance
                    index = row_term + color * versize + v.first;
                    BFS_mat[index] = v.second;
				}
				for(vector<int>::iterator it = adjacency_mat[v.first][color].begin(); it != adjacency_mat[v.first][color].end(); ++it) {
					if (!cyclefound && *it == curr_vertex) {
						// first self loop; shortest path from curr_vertex to itself found
                        index = row_term + color * versize + curr_vertex;
                        BFS_mat[index] = v.second  + 1;
						cyclefound = true;
						continue;
					}
					if (!flag[*it]) {
                        // push all neighbouring nodes with same color
                        // into the queue and mark them visited.
						q.push(make_pair(*it, v.second + 1));
						flag[*it] = true;
					}
				}
			}
		}
	}
	return;
}


void save_BFS_matrix(int* BFS_mat, int* offset, int versize) {
    FILE* fp = fopen("../log/BFS_mat.txt", "w");
    fwrite(BFS_mat, sizeof(int), versize * versize * NCOLORS, fp);
    fclose(fp);
    return;
}


void read_BFS_matrix(int* offset, int versize) {
    FILE* fp = fopen("../log/BFS_mat.txt", "r");
    int *arr = new int[NCOLORS * versize];
    fseek(fp, NCOLORS * versize * sizeof(int), SEEK_SET);
    fread(arr, sizeof(int), NCOLORS * versize, fp);
    for (int i = 0; i < NCOLORS * versize; i++)
        cout << arr[i] << " ";
    cout << endl;
    fclose(fp);
}


int main(int argc, char* argv[]) {
    freopen("../data/graph.txt", "r", stdin);
    int versize, edgesize;
    cin >> versize >> edgesize;

    int* attribute1 = new int[versize];
    string* attribute2 = new string[versize];
    string* attribute3 = new string[versize];
    string* attribute4 = new string[versize];

    // store the attributes
    for (int i = 0; i < versize; i++)
        cin >> attribute1[i] >> attribute2[i]
            >> attribute3[i] >> attribute4[i];

    vector< vector< vector<int> > > adjacency_mat(versize, vector< vector<int> >(NCOLORS));
    int u, v;
    char color;
    for (int i = 0; i < edgesize; i++) {
        cin >> u >> v >> color;
        adjacency_mat[u - 1][color - 'a'].push_back(v - 1);
    }
    int *BFS_mat = new int[versize * NCOLORS * versize];
    build_BFS_matrix(BFS_mat, versize, adjacency_mat);
    int* offset = new int[NCOLORS];
    save_BFS_matrix(BFS_mat, offset, versize);
    //read_BFS_matrix(offset, versize);

    string u_attr, v_attr, query;
    int query_size;
    double total_query_time = 0;
    cin >> query_size;
    int nq = query_size;
    FILE* fp = fopen("../log/BFS_mat.txt", "r");
    int* req_nodes = new int[versize * NCOLORS];
    while (query_size--) {
        cin >> u_attr >> v_attr >> query;
        clock_t time = clock();
        // Execute the query here
        vector<int> match_u = \
            find_candidate_nodes(u_attr, attribute1, attribute2, attribute3, attribute4, versize);
        vector<int> match_v = \
            find_candidate_nodes(v_attr, attribute1, attribute2, attribute3, attribute4, versize);
        int match_u_size = match_u.size();
        int match_v_size = match_v.size();
        bool pairsfound = false;
        vector<string> query_str = split_query_string(query);
        for (int i = 0; i < match_u_size; i++) {
            for (int j = 0; j < match_v_size; j++) {
                if (query_result(req_nodes, match_u[i], match_v[j], query_str, versize, fp)) {
                    pairsfound = true;
                    //cout << "(" << match_u[i] + 1 << "," << match_v[j] + 1 << ") ";
                }
            }
        }
        time = clock() - time;
        total_query_time  += (double)time / CLOCKS_PER_SEC;
    }
    fclose(fp);
	return 0;
}
