#include <bits/stdc++.h>
#include <chrono>

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


bool check_satisfiability(int u, int v, int dist, string regex_op) {
    if (regex_op.size() == 0) {
        // empty string
        return (dist == 1);
    } else if (regex_op == "+") {
        return true;
    } else {
        // c<k type
        int k = 0;
        for (int i = 0; i < regex_op.size(); i++) {
            if (!isalnum(regex_op[i]))
                continue;
            k = k * 10 + regex_op[i] - '0';
        }
        return (dist <= k);
    }
    return false;
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


unordered_set<int> find_next_set(
        int versize,
        unordered_set<int> &curr_set,
        vector< vector< pair<int, char> > > &adj,
        char color,
        string op,
        unordered_map<int, vector< vector< pair<int, int> > > > &color_table) {
    /*
     * Finds all vertices that are reachable from the nodes
     * of `curr_set` with given `color` edges and satisfying
     * the `op` constraints
     */
    int limit = find_limit(op);
    unordered_set<int> valid_set;
    int curr_vertex;
    for (auto it = curr_set.begin(); it != curr_set.end(); it++) {
        curr_vertex = *it;
        auto neighbors = color_table.find(curr_vertex);
        if (neighbors != color_table.end()) {
            auto tmp = neighbors->second;
            auto candidates = *(tmp.begin() + (color - 'a'));
            for (auto nbr = candidates.begin(); nbr != candidates.end(); nbr++) {
                pair<int, int> edge = *nbr;
                int dist = edge.second;
                if (dist <= limit) {
                    valid_set.insert(edge.first);
                }
            }
        } else {
            queue< pair<int, int> > BFS_queue;
            BFS_queue.push(make_pair(curr_vertex, 0));
            vector<bool> visited(versize, false);
            visited[curr_vertex] = true;

            while (!BFS_queue.empty()) {
                pair<int, int> root = BFS_queue.front();
                BFS_queue.pop();
                // insert into queue all unvisited vertices that are
                // connected with valid colored edges from the root
                if (root.second <= limit && root.second > 0) {
                    valid_set.insert(root.first);
                }
                for (auto nbr = adj[root.first].begin(); nbr != adj[root.first].end(); nbr++) {
                    pair<int, int> edge = *nbr;
                    if (!visited[edge.first]) {
                        if (edge.second == color) {
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


unordered_set<int> find_intersection(unordered_set<int> set1, unordered_set<int> set2) {
    unordered_set<int> result;
    if (set1.size() <= set2.size()) {
        for (auto it = set1.begin(); it != set1.end(); it++) {
            if (set2.find(*it) != set2.end())
                result.insert(*it);
        }
    } else {
        for (auto it = set2.begin(); it != set2.end(); it++) {
            if (set1.find(*it) != set1.end())
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
        vector<vector<pair<int, char> > > &rev_adj,
        unordered_map<int, vector< vector< pair<int, int> > > > &color_table_forward,
        unordered_map<int, vector< vector< pair<int, int> > > > &color_table_backward) {
    /*
     * Main function which performs bidirectional BFS traversal
     */
    unordered_set<int> start_reachable;
    unordered_set<int> start_reachable_next;
    unordered_set<int> result;

    start_reachable.insert(start);
    int regex_len = regex_colors.size();

    for (int i = 0; i < regex_len; i++) {
        start_reachable_next = find_next_set(versize, start_reachable, adj, regex_colors[i], regex_ops[i], color_table_forward);
        if (start_reachable_next.size() == 0)
            return result;
        start_reachable = start_reachable_next;
    }
    return find_intersection(start_reachable, end_nodes);
}


void evaluate_query(
        int versize,
        unordered_set<int> begin_nodes,
        unordered_set<int> end_nodes,
        string regex,
        vector< vector< pair<int, char> > > &adj,
        vector< vector< pair<int, char> > > &rev_adj,
        unordered_map<int, vector< vector< pair<int, int> > > > &color_table_forward,
        unordered_map<int, vector< vector< pair<int, int> > > > &color_table_backward) {
    /*
     * Utility Function to Print the query result
     */
    vector<char> regex_colors;
    vector<string> regex_ops;
    split_regex(regex, regex_colors, regex_ops);
    int start_node, end_node;

    //cout << "------------------------------------------------" << endl;
    //for (auto it=color_table_forward.begin(); it != color_table_forward.end(); it++) {
        //cout << "Vertex: " << it->first << endl;
        //auto nbrs = it->second;
        //for (auto it2 = nbrs.begin(); it2 != nbrs.end(); it2++) {
            //for (auto it3 = (*it2).begin(); it3 != (*it2).end(); it3++) {
                //cout << (*it3).first << " -- " << (*it3).second << endl;
            //}
        //}
    //}
    //cout << "------------------------------------------------" << endl;

    for (auto it1 = begin_nodes.begin(); it1 != begin_nodes.end(); it1++) {
        start_node = *it1;
        unordered_set<int> reachable_set = BFS(versize, *it1, end_nodes, regex_colors, regex_ops, adj, rev_adj, color_table_forward, color_table_backward);
        for (auto it2 = reachable_set.begin(); it2 != reachable_set.end(); it2++) {
            cout << "(" << *it1 << ", " << *it2 << ")" << endl;
        }
    }

    /********************DEBUG***************************/
    //unordered_set<int> reachable_set = BFS(versize, 0, end_nodes, regex_colors, regex_ops, adj, rev_adj, color_table_forward, color_table_backward);
    //for (auto it2 = reachable_set.begin(); it2 != reachable_set.end(); it2++) {
        //cout << "(" << 0 << ", " << *it2 << ")" << endl;
    //}
    /********************DEBUG***************************/
    return;
}

unordered_set<int> monte_carlo(int versize, vector< vector< pair<int, char> > > &adj) {
    unordered_set<int> result;
    for (int i = 19; i < 1000; i += 29)
        result.insert(i);
    //result.insert(0);
    //result.insert(2);
    return result;
}


unordered_map<int, vector< vector< pair<int, int> > > > partial_BFS_mat_build(
        int versize,
        vector< vector< pair<int, char> > > &adj) {

    unordered_set<int> popular_nodes = monte_carlo(versize, adj);
    unordered_map<int, vector< vector< pair<int, int> > > > result;
    int curr_vertex;
    for (auto it = popular_nodes.begin(); it != popular_nodes.end(); it++) {
        curr_vertex = *it;
        vector< vector< pair<int, int> > > nbr_list(NCOLORS);
        for (int col = 0; col < NCOLORS; col++) {
            // Perform BFS using a queue of (node, distance) pairs
            queue< pair<int,int> > q;
            q.push(make_pair(curr_vertex, 0));
            vector<bool> flag(versize, 0);
            flag[curr_vertex] = true;
            bool cyclefound = false;
            while (!q.empty()) {
                pair<int, int> v = q.front();
                q.pop();
                if (v.first != curr_vertex) {
                    // new vertex found! Store its distance
                    nbr_list[col].push_back(make_pair(v.first, v.second));
                }
                for (auto it = adj[v.first].begin(); it != adj[v.first].end(); ++it) {
                    pair<int, int> edge = *it;

                    /********************DEBUG***************************/
                    //cout << curr_vertex << " -- " << edge.first << " -- " << edge.second - 'a' << "  ***" << endl;
                    /********************DEBUG***************************/

                    if (edge.second - 'a' == col) {
                        if (!cyclefound && edge.first == curr_vertex) {
                            // first self loop; shortest path from curr_vertex to itself found
                            nbr_list[col].push_back(make_pair(curr_vertex, v.second + 1));
                            cyclefound = true;
                            continue;
                        }
                        if (!flag[edge.first]) {
                            // push all unvisited vertices connected to the node
                            // with current color into the queue
                            // Also, distance is increased by 1
                            q.push(make_pair(edge.first, v.second + 1));
                            // mark the node visited
                            flag[edge.first] = true;
                        }
                    }
                }
            }
        }
        result[curr_vertex] = nbr_list;
    }
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

    vector< vector< pair<int, char> > > adj, rev_adj;
    adj.resize(versize);
    rev_adj.resize(versize);
    int u, v;
    char color;
    for (int i = 0; i < edgesize; i++) {
        cin >> u >> v >> color;
        adj[u - 1].push_back(make_pair(v - 1, color));
        rev_adj[v - 1].push_back(make_pair(u - 1, color));
    }

    // Build the partial index
    unordered_map<int, vector< vector< pair<int, int> > > > partial_BFS_mat_forward = partial_BFS_mat_build(versize, adj);
    unordered_map<int, vector< vector< pair<int, int> > > > partial_BFS_mat_backward = partial_BFS_mat_build(versize, rev_adj);

    string uatt, vatt, regex;
    int querysize;
    int num_queries;

    cin >> querysize;
    vector<int> time_record;

    /********************DEBUG***************************/
    //querysize = 1;
    /********************DEBUG***************************/

    while (querysize--) {
        // perform query
        num_queries += 1;
        cin >> uatt >> vatt >> regex;
        auto start = std::chrono::high_resolution_clock::now();
        vector<int> begin = find_candidate_nodes(uatt, att1, att2, att3, att4, versize);
        vector<int> end = find_candidate_nodes(vatt, att1, att2, att3, att4, versize);
        unordered_set<int> begin_nodes(begin.begin(), begin.end());
        unordered_set<int> end_nodes(end.begin(), end.end());
        evaluate_query(versize, begin_nodes, end_nodes, regex, adj, rev_adj, partial_BFS_mat_forward, partial_BFS_mat_backward);
        auto diff = std::chrono::high_resolution_clock::now() - start;
        auto t1 = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
        cout << "Time Taken: " << t1.count() << endl;
    }
    return 0;
}
