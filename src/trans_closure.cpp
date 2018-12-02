#include <bits/stdc++.h>
#include <chrono>

using namespace std;
#define INF     INT_MAX
int n_colors = 0;

struct pair_hash {
    inline std::size_t operator()(const std::pair<int,int> & v) const {
        return v.first*31+v.second;
    }
};


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
        vector<int> &att4,
        vector<int> &att5,
        vector<int> &att6,
        int versize
        ) {

    vector<string> attrib_ops = find_attribs(matstr);
    vector<int> match_u;

    for (int i = 0; i < versize; i++) {
        if (matchnum(att1[i], attrib_ops[0], attrib_ops[1])
            && matchstr(att2[i], attrib_ops[2], attrib_ops[3])
            && matchstr(att3[i], attrib_ops[4], attrib_ops[5])
            && matchnum(att4[i], attrib_ops[6], attrib_ops[7])
            && matchnum(att5[i], attrib_ops[8], attrib_ops[9])
            && matchnum(att6[i], attrib_ops[10], attrib_ops[11])) {
            // node i staisfies all attribute constraints
            // so it is an eligible candidate for the query
            match_u.push_back(i);
        }
    }
    return match_u;
}


void BFS_matrix_build(vector< vector< vector<int> > > &adj, int versize, int*** M) {
    for (int i = 0; i < versize; i++) {
        int curr_vertex = i;
        for (int col = 0; col < n_colors; col++) {
            // Perform BFS using a queue of (node, distance) pairs
            queue< pair<int,int> > q;
            q.push(make_pair(curr_vertex, 0));
            vector<bool> flag(versize, 0);
            flag[curr_vertex] = true;
            //bool cyclefound = false;
            while (!q.empty()) {
                pair<int, int> v = q.front();
                q.pop();
                if (v.first != curr_vertex) {
                    // new vertex found! Store its distance
                    M[col][curr_vertex][v.first] = v.second;
                }
                vector<int>::iterator it;
                for(it = adj[v.first][col].begin(); it != adj[v.first][col].end(); ++it) {
                    //if (!cyclefound && *it == curr_vertex) {
                        // first self loop; shortest path from curr_vertex to itself found
                        //M[col][curr_vertex][curr_vertex] = v.second + 1;
                        //cyclefound = true;
                        //continue;
                    //}
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
    //for (int c = 0; c < n_colors; c++) {
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

typedef struct Triplets {
    Triplets(int _u, int _v, int _dist) {
        u = _u;
        v = _v;
        dist = _dist;
    }
    int u;
    int v;
    int dist;
} triplets;

void save_BFS_matrix(int*** BFS_mat, int versize, int* color_frequency, FILE* fp) {
    for (int c = 0; c < n_colors; c++) {
        vector<triplets> connected_nodes;
        // find all connected pairs
        for (int u = 0; u < versize; u++) {
            for (int v = 0; v < versize; v++) {
                if (BFS_mat[c][u][v] != INF) {
                    // *******************************************************************************
                    // DEBUG
                    // *******************************************************************************
                    //cout << u << " - " << v << endl;
                    //cout << BFS_mat[c][u][v] << endl;
                    // *******************************************************************************
                    // *******************************************************************************
                    //cout << u << " -- " << v << endl;
                    triplets T(u, v, BFS_mat[c][u][v]);
                    //cout << T.u << " -- " << T.v << " -- " << T.dist << endl;
                    connected_nodes.push_back(T);
                }
            }
        }
        //cout << count << " ^^" << endl;
        //cout << "Mark1" << endl;
        color_frequency[c] = connected_nodes.size();
        //cout << "--> " << color_frequency[c] << endl;
        //sort(connected_nodes.begin(), connected_nodes.end(),
                //[](triplets a, triplets b) {
                    //return (a.dist <= b.dist);
                //});

        for (auto it = connected_nodes.begin(); it != connected_nodes.end(); it++) {
            triplets T = *it;
            fwrite(&(T.u), sizeof(int), 1, fp);
            fwrite(&(T.v), sizeof(int), 1, fp);
            fwrite(&(T.dist), sizeof(int), 1, fp);
        }
    }
    return;
}

void debug(int*** M, int versize) {
    int k = 0;
    for (int c = 0; c < n_colors; c++) {
        for (int u = 0; u < versize; u++) {
            for (int v = 0; v < versize; v++) {
                if (M[c][u][v] != INF) {
                    k++;
                }
            }
            cout << "Node: " << u << ", Color: " << c << " Size: " << k << endl;
            k = 0;
        }
    }
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


int find_limit(string s) {
    int len = s.size();
    int limit = 0;
    if (len == 0) {
        limit = 1;
    } else if (len == 1) {
        limit = INF;
    } else {
        for (int i = 0; i < s.size(); i++) {
            if (!isalnum(s[i]))
                continue;
            limit = limit * 10 + s[i] - '0';
        }
    }
    return limit;
}


unordered_set< pair<int, int>, pair_hash> recursive_search(
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
    //cout << index_of_min_freq_color << endl;
    int color = regex_colors[index_of_min_freq_color] - 'a';

    // extract the data for `color`
    int N = color_frequency[color];
    int* color_table = (int*)malloc(sizeof(int) * 3 * N);
    find_color_pairs(color_table, color, color_frequency, fp);

    // filter all pairs of vertices satisfying the
    // constraint due to corresponding op symbol
    int limit = find_limit(regex_ops[index_of_min_freq_color]);
    vector<int> candidate_begin;
    vector<int> candidate_end;
    for (int i = 0; i < 3 * N; i += 3) {
        if (!(color_table[i + 2] > limit)) {
            candidate_begin.push_back(color_table[i]);
            candidate_end.push_back(color_table[i + 1]);
        }
    }
    int len = regex_colors.size();
    unordered_set< pair<int, int>, pair_hash > res;
    if (candidate_begin.size() == 0)
        return res;

    if (len == 1) {
        int L = candidate_begin.size();
        for (int i = 0; i < L; i++) {
            if (begin_nodes.find(candidate_begin[i]) != begin_nodes.end()
                    && end_nodes.find(candidate_end[i]) != end_nodes.end()) {
                res.insert(make_pair(candidate_begin[i], candidate_end[i]));
            }
        }
        return res;
    } else if (index_of_min_freq_color == 0 && len >= 2) {
        int L = candidate_begin.size();
        unordered_set<int> next_begin_nodes;
        unordered_map<int, vector<int> > current_edges;
        // check for intersection between begin_nodes & candidate_begin
        for (int i = 0; i < L; i++) {
            if (begin_nodes.find(candidate_begin[i]) != begin_nodes.end()) {
                next_begin_nodes.insert(candidate_end[i]);
                current_edges[candidate_end[i]].push_back(candidate_begin[i]);
            }
        }
        // Recursively call find path on remaining query
        vector<char> new_regex_colors(regex_colors.begin() + 1, regex_colors.end());
        vector<string> new_regex_ops(regex_ops.begin() + 1, regex_ops.end());
        unordered_set< pair<int, int>, pair_hash > remaining_path =
            recursive_search(next_begin_nodes, end_nodes, color_frequency, new_regex_colors, new_regex_ops, fp);
        // update the paths by replacing path start with corresponding
        // node from `current_edges`
        //unordered_set< pair<int, int>, pair_hash >::iterator it;
        for (auto it = remaining_path.begin(); it != remaining_path.end(); it++) {
            int old_start = (*it).first;
            vector<int> new_starts = current_edges[old_start];
            int d = new_starts.size();
            for (int j = 0; j < d; j++) {
                res.insert(make_pair(new_starts[j], (*it).second));
            }
        }
    } else if (index_of_min_freq_color == len - 1) {
        int L = candidate_end.size();
        unordered_set<int> next_end_nodes;
        unordered_map<int, vector<int> > current_edges;
        // check for intersection between end_nodes & candidate_end
        for (int i = 0; i < L; i++) {
            if (end_nodes.find(candidate_end[i]) != end_nodes.end()) {
                next_end_nodes.insert(candidate_begin[i]);
                current_edges[candidate_begin[i]].push_back(candidate_end[i]);
            }
        }
        // Recursively call find path on remaining query
        vector<char> new_regex_colors(regex_colors.begin(), regex_colors.end() - 1);
        vector<string> new_regex_ops(regex_ops.begin(), regex_ops.end() - 1);
        unordered_set< pair<int, int>, pair_hash > remaining_path =
            recursive_search(begin_nodes, next_end_nodes, color_frequency, new_regex_colors, new_regex_ops, fp);
        // update the paths by replacing path start with corresponding
        // node from `current_edges`
        for (auto it = remaining_path.begin(); it != remaining_path.end(); it++) {
            int old_end = (*it).second;
            vector<int> new_ends = current_edges[old_end];
            int d = new_ends.size();
            for (int j = 0; j < d; j++) {
                res.insert(make_pair((*it).first, new_ends[j]));
            }
        }
    } else {
        int L = candidate_begin.size();
        unordered_set< pair<int, int>, pair_hash > current_edges;
        for (int i = 0; i < L; i++)
            current_edges.insert(make_pair(candidate_begin[i], candidate_end[i]));

        vector<char> begin_regex_color(regex_colors.begin(), regex_colors.begin() + index_of_min_freq_color);
        vector<string> begin_regex_ops(regex_ops.begin(), regex_ops.begin() + index_of_min_freq_color);
        vector<char> end_regex_color(regex_colors.begin() + index_of_min_freq_color + 1, regex_colors.end());
        vector<string> end_regex_ops(regex_ops.begin() + index_of_min_freq_color + 1, regex_ops.end());

        unordered_set<int> next_end_nodes(candidate_begin.begin(), candidate_begin.end());
        unordered_set<int> next_begin_nodes(candidate_end.begin(), candidate_end.end());

        unordered_set< pair<int, int>, pair_hash > path_above =
            recursive_search(begin_nodes, next_end_nodes, color_frequency, begin_regex_color, begin_regex_ops, fp);
        unordered_set< pair<int, int>, pair_hash > path_below =
            recursive_search(next_begin_nodes, end_nodes, color_frequency, end_regex_color, end_regex_ops, fp);

        for (auto it1 = path_above.begin(); it1 != path_above.end(); it1++) {
            for (auto it2 = path_below.begin(); it2 != path_below.end(); it2++) {
                int start = (*it1).second;
                int end = (*it2).first;
                if (current_edges.find(make_pair(start, end)) != current_edges.end()) {
                    res.insert(make_pair((*it1).first, (*it2).second));
                }
            }
        }
    }
    return res;
}


void evaluate_query(unordered_set<int> begin_nodes, unordered_set<int> end_nodes, int* color_frequency, string regex, FILE* fp) {
    vector<char> regex_colors;
    vector<string> regex_ops;
    split_regex(regex, regex_colors, regex_ops);
    unordered_set< pair<int, int>, pair_hash > query_res = recursive_search(begin_nodes, end_nodes, color_frequency, regex_colors, regex_ops, fp);
    //for (auto it = query_res.begin(); it != query_res.end(); it++) {
        //cout << "(" << (*it).first << ", " << (*it).second << ")" << endl;
    //}
    //cout << query_res.size() << endl;
    return;
}


int main(int argc, char* argv[]) {
    freopen(argv[1], "r", stdin);
    int versize, edgesize;
    cin >> versize >> edgesize >> n_colors;
    vector<int> att1(versize); //Year of birth
    vector<string> att2(versize); // Name
    vector<string> att3(versize); // Sex
    vector<int> att4(versize); // num_posts
    vector<int> att5(versize); // num_friends
    vector<int> att6(versize); // UID
    for (int i = 0; i < versize; i++) {
        cin >> att1[i] >> att2[i] >> att3[i] >> att4[i] >> att5[i] >> att6[i];
    }

    vector< vector< vector<int> > > adj(versize, vector< vector<int> >(n_colors));
    int u, v;
    char color;
    for (int i = 0; i < edgesize; i++){
        cin >> u >> v >> color;
        adj[u][color - 'a'].push_back(v);
    }

    auto start = std::chrono::high_resolution_clock::now();
    int*** BFS_mat = (int***)malloc(n_colors * sizeof(int**));
    for (int i = 0; i < n_colors; i++) {
        BFS_mat[i] = (int**)malloc(versize * sizeof(int*));
        for (int j = 0; j < versize; j++) {
            BFS_mat[i][j] = (int*)malloc(versize * sizeof(int));
            for (int k = 0; k < versize; k++) {
                BFS_mat[i][j][k] = INF;
            }
        }
    }

    BFS_matrix_build(adj, versize, BFS_mat);
    debug(BFS_mat, versize);

    int* color_frequency = (int*)malloc(n_colors * sizeof(int));
    FILE* fp = fopen("../BFS_index/trans_closure", "r+");
    save_BFS_matrix(BFS_mat, versize, color_frequency, fp);
    auto diff = std::chrono::high_resolution_clock::now() - start;
    auto prepro_time = std::chrono::duration_cast<std::chrono::milliseconds>(diff);

    string uatt, vatt, regex;
    int querysize;

    cin >> querysize;
    cout << "Preprocessing_Time: " << prepro_time.count() << endl;
    while (querysize--) {
        // perform query
        cin >> uatt >> vatt >> regex;
        cout << versize << ", " << uatt << ", " << vatt << ", " << regex << ", ";
        start = std::chrono::high_resolution_clock::now();
        vector<int> begin = find_candidate_nodes(uatt, att1, att2, att3, att4, att5, att6, versize);
        vector<int> end = find_candidate_nodes(vatt, att1, att2, att3, att4, att5, att6, versize);
        unordered_set<int> begin_nodes(begin.begin(), begin.end());
        unordered_set<int> end_nodes(end.begin(), end.end());
        evaluate_query(begin_nodes, end_nodes, color_frequency, regex, fp);
        diff = std::chrono::high_resolution_clock::now() - start;
        auto eval_time = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
        cout << eval_time.count() << endl;
    }
    return 0;
}
