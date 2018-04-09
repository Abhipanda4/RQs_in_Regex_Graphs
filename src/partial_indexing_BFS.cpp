#include <bits/stdc++.h>
#include <chrono>

using namespace std;
#define  INF                   INT_MAX
#define  EXP                   2.71828

int hit = 0;
int miss = 0;
int called = 0;
int NUM_SIMULATIONS = 100;
int THRESHOLD_NUM_NODES = 1000;
int NCOLORS = -1;

struct pair_hash {
    /*
     * Custom hash function for hashing pairs
     */
    inline std::size_t operator()(const std::pair<int,int> & v) const {
        return v.first * 37 + v.second;
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
    /*
     * Function to determine all the nodes that satisfy
     * the queery predicate
     */

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

int find_limit(string s) {
    /*
     * All regular expressions in our grammar can be converted into:
     * c <= K form since the nodes being considered will always have
     * atleast one connection(c > 1 always).
     * This function finds the upper limit for how many edges can be considered(K)
     */
    int len = s.size();
    int limit = 0;
    if (len == 0) {
        // c -> c <= 1
        limit = 1;
    } else if (len == 1) {
        // c+ -> c <= INF
        limit = INF;
    } else {
        // c <= k form already
        for (int i = 0; i < s.size(); i++) {
            if (!isalnum(s[i]))
                continue;
            limit = limit * 10 + s[i] - '0';
        }
    }
    return limit;
}


unordered_set<int> find_next_set(
        int versize,
        unordered_set<int> &curr_set,
        vector< vector< pair<int, char> > > &adj,
        char color,
        string op,
        unordered_map< pair<int, char>, vector< pair<int, int> >, pair_hash> &color_table
        ) {
    /*
     * Finds all vertices that are reachable from the nodes
     * of `curr_set` with given `color` edges and satisfying
     * the `op` constraints
     */

    int limit = find_limit(op);
    unordered_set<int> valid_set;
    int curr_vertex;
    for (auto it = curr_set.begin(); it != curr_set.end(); it++) {
        /********************DEBUG***************************/
        ::called += 1;
        /********************DEBUG***************************/
        curr_vertex = *it;
        //cout << curr_vertex << " ** " << color << endl;
        auto neighbors = color_table.find(make_pair(curr_vertex, color));
        if (neighbors != color_table.end()) {
            // we have found a popular pair

            /********************DEBUG***************************/
            ::hit += 1;
            /********************DEBUG***************************/

            auto adjacency = neighbors->second;
            int N = adjacency.size();
            int i = 0;
            while (adjacency[i].second <= limit && i < N) {
                valid_set.insert(adjacency[i].first);
                i += 1;
            }
        } else {
            ::miss += 1;
            queue< pair<int, int> > BFS_queue;
            BFS_queue.push(make_pair(curr_vertex, 0));
            vector<bool> visited(versize, false);
            visited[curr_vertex] = true;

            while (!BFS_queue.empty()) {
                pair<int, int> root = BFS_queue.front();
                BFS_queue.pop();
                if (root.second > 0) {
                    valid_set.insert(root.first);
                }
                for (auto nbr = adj[root.first].begin(); nbr != adj[root.first].end(); nbr++) {
                    pair<int, int> edge = *nbr;
                    if (!visited[edge.first]) {
                        if (edge.second == color && root.second < limit) {
                            // insert into queue all unvisited vertices that are
                            // connected with valid colored edges from the root
                            BFS_queue.push(make_pair(edge.first, root.second + 1));
                            visited[edge.first] = true;
                        }
                    }
                }
            }
        }
    }
    /********************DEBUG***************************/
    //cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    //for (auto it = valid_set.begin(); it != valid_set.end(); it++)
        //cout << *it << endl;
    //cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    /********************DEBUG***************************/
    return valid_set;
}


template <typename T>
T  find_intersection(T &set1, T &set2) {
    /*
     * A utility function to find intersection of 2 sets.
     * Iterates through the smaller set for improved efficiency.
     * Hence, time complexity:
     * O( min{size(set1), size(set2)} )
     */
    if (set1.size() > set2.size()) {
        return find_intersection(set2, set1);
    }
    T result;
    for (auto it = set1.begin(); it != set1.end(); it++) {
        if (set2.find(*it) != set2.end()) {
            result.insert(*it);
        }
    }
    return result;

}


unordered_set<int> BFS(
        int versize,
        int start,
        unordered_set<int> end_nodes,
        vector<char> &regex_colors,
        vector<string> &regex_ops,
        vector<vector<pair<int, char> > > &adj,
        unordered_map< pair<int, char>, vector< pair<int, int> >, pair_hash> &color_table
        ) {
    /*
     * Main function which performs BFS traversal
     */
    unordered_set<int> start_reachable;
    unordered_set<int> start_reachable_next;
    unordered_set<int> result;

    start_reachable.insert(start);
    int regex_len = regex_colors.size();

    for (int i = 0; i < regex_len; i++) {
        start_reachable_next = find_next_set(versize, start_reachable, adj, regex_colors[i], regex_ops[i], color_table);
        if (start_reachable_next.size() == 0)
            return result;
        start_reachable = start_reachable_next;
    }
    return find_intersection(start_reachable, end_nodes);
}

void BFS_util(
        int versize,
        unordered_set<int> &begin_nodes,
        unordered_set<int> &end_nodes,
        vector<char> &regex_colors,
        vector<string> &regex_ops,
        vector< vector< pair<int, char> > > &adj,
        unordered_map< pair<int, char>, vector< pair<int, int> >, pair_hash> &color_table,
        bool is_reverse
        ) {
    /*
     * Utility function which calls the BFS function to evaluate query
     * and also prints the result
     */
    int start_node;
    unordered_set<int> reachable_set;
    int ans = 0;
    for (auto it1 = begin_nodes.begin(); it1 != begin_nodes.end(); it1++) {
        start_node = *it1;
        reachable_set = BFS(versize, *it1, end_nodes, regex_colors, regex_ops, adj, color_table);
        for (auto it2 = reachable_set.begin(); it2 != reachable_set.end(); it2++) {
            if (!is_reverse) {
                //cout << "(" << *it1 << ", " << *it2 << ")" << endl;
                ans += 1;
            } else {
                //cout << "(" << *it2 << ", " << *it1 << ")" << endl;
                ans += 1;
            }
        }
    }
    //cout << ans << endl;
}

void evaluate_query(
        int versize,
        unordered_set<int> &begin_nodes,
        unordered_set<int> &end_nodes,
        string regex,
        vector< vector< pair<int, char> > > &adj,
        vector< vector< pair<int, char> > > &rev_adj,
        unordered_map< pair<int, char>, vector< pair<int, int> >, pair_hash> &color_table_forward,
        unordered_map< pair<int, char>, vector< pair<int, int> >, pair_hash> &color_table_backward
        ) {
    /*
     * Utility Function to extract colors and ops from regex and decide the direction of BFS
     */
    vector<char> regex_colors;
    vector<string> regex_ops;
    split_regex(regex, regex_colors, regex_ops);
    if (begin_nodes.size() < end_nodes.size()) {
        // search in forward direction
        BFS_util(versize, begin_nodes, end_nodes, regex_colors, regex_ops, adj, color_table_forward, false);
    } else {
        // search in backward direction
        // reverse the colors and ops
        reverse(regex_colors.begin(), regex_colors.end());
        reverse(regex_ops.begin(), regex_ops.end());
        BFS_util(versize, end_nodes, begin_nodes, regex_colors, regex_ops, rev_adj, color_table_backward, true);
    }
    return;
}

/*
 ******************************************************************************
 **-----------------------MONTE CARLO FRAMEWORK START--------------------------
 ******************************************************************************
 */

template <typename T>
vector<int> argsort(vector<T> &X) {
    int N = X.size();
    vector<int> sorted_indices(N);
    iota(sorted_indices.begin(), sorted_indices.end(), 0);

    sort(sorted_indices.begin(), sorted_indices.end(),
            [&X](int i, int j) -> bool {return X[i] < X[j];});
    return sorted_indices;
}

template <typename T>
T find_Kth_largest(vector<T> &X, int k) {
    /*
     * Function to find kth largest number in a vector
     */
    sort(X.begin(), X.end(), greater<int>());
    return *(X.begin() + k - 1);
}

template <typename T>
pair<int, char> find_largest(vector< vector<T> > &X) {
    /*
     * Returns the (row, col) pair with largest value in a 2D vector matrix
     */
    int rows = X.size();
    int cols = X[0].size();
    T largest_till_now = X[0][0];
    //cout << largest_till_now << " ** " << endl;
    int u = 0;
    int c = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (X[i][j] > largest_till_now) {
                largest_till_now = X[i][j];
                u = j;
                c = i;
            }
        }
    }
    X[c][u] = -1;
    //cout << largest_till_now << " -- " << u << " -- " << (char)(c + 'a') << endl;
    return make_pair(u, (char)(c + 'a'));
}

unordered_set<int> find_reachable_nodes(
        int node,
        int color,
        vector< vector< pair<int, char> > > &rev_adj,
        vector<bool> &visited
        ) {
    /*
     * This function returns all nodes that are reachable fron `node`
     * only using edges of `color` label
     */
    unordered_set<int> valid_set;
    queue<int> BFS_queue;
    BFS_queue.push(node);
    visited[node] = true;

    while (!BFS_queue.empty()) {
        int root = BFS_queue.front();
        // anything entering the queue is reachable !!
        valid_set.insert(root);
        BFS_queue.pop();
        for (auto nbr = rev_adj[root].begin(); nbr != rev_adj[root].end(); nbr++) {
            pair<int, char> edge = *nbr;
            if (!visited[edge.first]) {
                if (edge.second - 'a' == color) {
                    BFS_queue.push(edge.first);
                    visited[edge.first] = true;
                }
            }
        }
    }
    return valid_set;
}

void assign_min_label(
        int root_node,
        int color,
        vector< vector< pair<int, char> > > &rev_adj,
        vector<float> &node_labels,
        vector< vector<float> > &min_labels,
        vector<bool> &visited
        ) {
    /*
     * Perform a BFS traversal to find all reachable nodes
     * Assign the label for root node to all reachable nodes
     * since it will be the lowest
     */
    unordered_set<int> reachable_set = find_reachable_nodes(root_node, color, rev_adj, visited);

    for (int node : reachable_set) {
        assert(min_labels[color][node] == -1);
        min_labels[color][node] = node_labels[root_node];
    }
}


vector< vector<float> > compute_min_label(
        int versize,
        vector<float> &node_labels,
        vector< vector< pair<int, char> > > &rev_adj
        ) {
    /*
     * Function to compute the minimum label for each node
     * Min-Label is defined as: minL(v) = min{L(x) | x is reachable from v}
     * Time Complexity: O(m + n)
     */
    vector<int> sorted_nodes = argsort(node_labels);
    vector<float> tmp(versize, -1);
    vector< vector<float> > min_labels(NCOLORS, tmp);
    for (int col = 0; col < NCOLORS; col++) {
        vector<bool> visited(versize, false);
        for (auto it = sorted_nodes.begin(); it != sorted_nodes.end(); it++) {
            int node = *it;
            if (!visited[node]) {
                assign_min_label(node, col, rev_adj, node_labels, min_labels, visited);
            }
        }
    }
    return min_labels;
}


vector< pair<int, char> > monte_carlo(
        int versize,
        vector< vector< pair<int, char> > > &adj,
        vector< vector< pair<int, char> > > &rev_adj
        ) {
    /*
     * A randomized framework to select the most popular
     * (node, color) pairs. Uses number of reachable nodes
     * as a measure of popularity.
     * The vanilla version of algorithm for estimating
     * size of reachabilty set can be found at:
     * https://www.sciencedirect.com/science/article/pii/S0022000097915348
     */
    vector< pair<int, char> > result;
    vector<float> node_labels(versize, 0);
    vector< vector< vector<float> > > min_label_store(NUM_SIMULATIONS);
    for (int k = 0; k < NUM_SIMULATIONS; k++) {
        // assign random no selected uniformly and independently from
        // (0, 1] to each vertex
        srand(k);
        //cout << "Running simulation " << k << "... " << endl;
        // A fancy way to generate an array of **UNIQUE** random numbers
        unordered_set<int> unique_rand;
        while (unique_rand.size() < versize) {
            unique_rand.insert(1 + rand() % 1000000);
        }
        int index = 0;
        for (auto it = unique_rand.begin(); it != unique_rand.end(); it++) {
            node_labels[index] = *it / 1000000.0;
            index += 1;
        }
        random_shuffle(node_labels.begin(), node_labels.end());
        // compute min_label for each (node, color) pair
        vector< vector<float> > min_labels = compute_min_label(versize, node_labels, rev_adj);
        min_label_store[k] = min_labels;
    }

    // Best estimate for size of reachability set is 1/t* - 1 where:
    // t* = (NUM_SIMULATIONS / e)th largest value among min_label_store[:][c][u]
    vector<float> tmp(versize);
    vector< vector<float> > best_labels(NCOLORS, tmp);
    for (int c = 0; c < NCOLORS; c++) {
        for (int u = 0; u < versize; u++) {
            vector<float> candidate_min_labels;
            //cout << c << " -- " << u << " -- ";
            for (int k = 0; k < NUM_SIMULATIONS; k++) {
                //cout << min_label_store[k][c][u] << " | ";
                candidate_min_labels.push_back(min_label_store[k][c][u]);
            }
            //cout << "****************************************************************" << endl;
            best_labels[c][u] = find_Kth_largest(candidate_min_labels, NUM_SIMULATIONS / EXP);
            //cout << best_labels[c][u] << endl;
        }
    }
    // compute size of reachability sets
    for (int c = 0; c < NCOLORS; c++) {
        for (int u = 0; u < versize; u++) {
            assert(best_labels[c][u] != 0);
            best_labels[c][u] = 1.0 / best_labels[c][u] - 1;

            /********************DEBUG***************************/
            //vector<bool> visited(versize, false);
            //int real_size = find_reachable_nodes(u, c,  adj, visited).size();
            //cout << "Actual: " << real_size << ", Predicted: " << best_labels[c][u] << endl;
            /********************DEBUG***************************/

        }
    }

    // select THRESHOLD_NUM_NODES pairs with largest reachability sets
    // cardinality for calculating partial transitive closure
    for (int i = 0; i < THRESHOLD_NUM_NODES; i++) {
        pair<int, char> popular_pair = find_largest(best_labels);
        result.push_back(popular_pair);
    }
    return result;
}

typedef struct Triplets {
    Triplets(int _u, int _c, int _num_nodes) {
        u = _u;
        c = _c;
        num_nodes = _num_nodes;
    }
    int u;
    int c;
    int num_nodes;
} triplets;

vector< pair<int, char> > find_real_pop_nodes(
        int versize,
        vector< vector< pair<int, char> > > &adj,
        vector< vector< pair<int, char> > > &rev_adj
        ) {
    vector<triplets> store;
    for (int c = 0; c < NCOLORS; c++) {
        for (int u = 0; u < versize; u++) {
            vector<bool> visited(versize);
            int num_nodes = find_reachable_nodes(u, c, adj, visited).size();
            triplets T(u, c, num_nodes);
            store.push_back(T);
        }
    }
    sort(store.begin(), store.end(),
            [](triplets a, triplets b) {
                return (a.num_nodes > b.num_nodes);
            });
    vector< pair<int, char> > result(THRESHOLD_NUM_NODES);
    for (int i = 0; i < THRESHOLD_NUM_NODES; i++) {
        result[i] = make_pair(store[i].u, (char)(store[i].c + 'a'));
    }
    //cout << result[1].first << " -- " << result[1].second << endl;
    return result;
}

unordered_map< pair<int, char>, vector< pair<int, int> >, pair_hash> partial_BFS_mat_build(
        int versize,
        vector< vector< pair<int, char> > > &adj,
        vector< vector< pair<int, char> > > &rev_adj) {
    /*
     * Function to build a (partial) lookup table.
     * The key to this table is a (node, color) pair;
     * the table stores all nodes reachable from key node
     * using only edges of key color. The keys are determined using
     * the montecarlo function
     */
    vector< pair<int, char> > popular_nodes = monte_carlo(versize, adj, rev_adj);
    /********************DEBUG***************************/
    //vector< pair<int, char> > real_popular_nodes = find_real_pop_nodes(versize, adj, rev_adj);
    //unordered_set<pair<int, char>, pair_hash> set1(popular_nodes.begin(), popular_nodes.end());
    //unordered_set<pair<int, char>, pair_hash> set2(real_popular_nodes.begin(), real_popular_nodes.end());
    //int num_intersection = find_intersection(set1, set2).size();
    //cout << "Num Intersection: " << num_intersection << endl;
    /********************DEBUG***************************/

    unordered_map< pair<int, char>, vector< pair<int, int> >, pair_hash> table;
    int decoy = 0;
    for (auto pop_node = popular_nodes.begin(); pop_node != popular_nodes.end(); pop_node++) {
        pair<int, char> key = *pop_node;
        //cout << "Node: " << key.first << " -- " << "Color: " << key.second << endl;
        int curr_vertex = key.first;
        int col = key.second - 'a';
        vector< pair<int, int> > res;
        // perform BFS using a queue of (node, distance) pairs
        queue< pair<int,int> > q;
        q.push(make_pair(curr_vertex, 0));
        vector<bool> visited(versize, 0);
        visited[curr_vertex] = true;
        //bool cyclefound = false;
        while (!q.empty()) {
            pair<int, int> v = q.front();
            q.pop();
            if (v.first != curr_vertex) {
                // new vertex found! Store its distance
                res.push_back(make_pair(v.first, v.second));
            }
            for (auto it = adj[v.first].begin(); it != adj[v.first].end(); it++) {
                pair<int, int> edge = *it;
                if (edge.second - 'a' == col) {
                    //if (!cyclefound && edge.first == curr_vertex) {
                        // first self loop; shortest path from curr_vertex to itself found
                        //res.push_back(make_pair(curr_vertex, v.second + 1));
                        //cyclefound = true;
                        //continue;
                    //}
                    if (!visited[edge.first]) {
                        // push all unvisited vertices connected to the node
                        // with current color into the queue
                        // Also, distance is increased by 1
                        q.push(make_pair(edge.first, v.second + 1));
                        // mark the node visited
                        visited[edge.first] = true;
                    }
                }
            }
        }
        if (res.size() == 0) {
            decoy += 1;
            continue;
        }
        sort(res.begin(), res.end(),
                [](pair<int, int> &a, pair<int, int> &b) {
                    return (a.second < b.second);
                });
        table[key] = res;
    }
    return table;
}

/*
 ******************************************************************************
 **-----------------------MONTE CARLO FRAMEWORK END----------------------------
 ******************************************************************************
 */


int main(int argc, char* argv[]) {
    for (int MC_nodes = 1; MC_nodes <= 5; MC_nodes++) {
        freopen(argv[1], "r", stdin);
        int versize, edgesize;

        cin >> versize >> edgesize >> NCOLORS;
        THRESHOLD_NUM_NODES = MC_nodes * versize * NCOLORS / 10;
        //cout << versize << " " << edgesize << " " << NCOLORS << " " << THRESHOLD_NUM_NODES << endl;
        vector<int> att1(versize); //Year of birth
        vector<string> att2(versize); // Name
        vector<string> att3(versize); // Sex
        vector<int> att4(versize); // num_posts
        vector<int> att5(versize); // num_friends
        vector<int> att6(versize); // UID
        for (int i = 0; i < versize; i++) {
            cin >> att1[i] >> att2[i] >> att3[i] >> att4[i] >> att5[i] >> att6[i];
        }

        vector< vector< pair<int, char> > > adj, rev_adj;
        adj.resize(versize);
        rev_adj.resize(versize);
        int u, v;
        char color;
        for (int i = 0; i < edgesize; i++) {
            cin >> u >> v >> color;
            //cout << u << " -- " << v << " -- " << color << endl;
            adj[u].push_back(make_pair(v, color));
            rev_adj[v].push_back(make_pair(u, color));
        }

        // Build the partial index
        auto start = std::chrono::high_resolution_clock::now();
        unordered_map< pair<int, char>, vector< pair<int, int> >, pair_hash>
            partial_BFS_mat_forward = partial_BFS_mat_build(versize, adj, rev_adj);
        unordered_map< pair<int, char>, vector< pair<int, int> >, pair_hash>
            partial_BFS_mat_backward = partial_BFS_mat_build(versize, rev_adj, adj);
        auto diff = std::chrono::high_resolution_clock::now() - start;
        auto prepro_time = std::chrono::duration_cast<std::chrono::milliseconds>(diff);

        string uatt, vatt, regex;
        int querysize;
        int num_queries;

        cin >> querysize;
        cout << "Preprocessing_Time: " << prepro_time.count() << endl;

        while (querysize--) {
            // perform query
            num_queries += 1;
            cin >> uatt >> vatt >> regex;
            cout << regex << ", ";
            start = std::chrono::high_resolution_clock::now();
            vector<int> begin = find_candidate_nodes(uatt, att1, att2, att3, att4, att5, att6, versize);
            vector<int> end = find_candidate_nodes(vatt, att1, att2, att3, att4, att5, att6, versize);
            unordered_set<int> begin_nodes(begin.begin(), begin.end());
            unordered_set<int> end_nodes(end.begin(), end.end());
            evaluate_query(versize, begin_nodes, end_nodes, regex, adj, rev_adj,
                    partial_BFS_mat_forward, partial_BFS_mat_backward);
            diff = std::chrono::high_resolution_clock::now() - start;
            auto t1 = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
            cout << t1.count() << ", ";
            cout << ::hit << ", " << ::miss << endl;
        }
    }
    return 0;
}
