#include <vector>
#include <set>
#include <queue>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cmath>
#include <chrono>

using namespace std;

#define u64 long long
#define NCOLORS 10
#define INF 1000000000
#define MAXVER 20000

bool matchstr(string att, string op, string val) {
	if (val == "_") {
		return true;
	}
	else if (op == "==") {
		return val == att? true: false;
	}
	else {
		return val != att? true: false;
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


vector<string> breakatt(string s) {
	vector<string> attop;

	int i = 0;
	int len = s.size();
	while (i < len) {
		string temp;

		while (i < len && s[i] == '_') {
			attop.push_back("_");
			i++;
		}

		if (i == len) {
			break;
		}

		while (!isalnum(s[i])) {
			temp += s[i];
			i++;
		}
		attop.push_back(temp);
		temp = "";

		while (i < len && isalnum(s[i])) {
			temp += s[i];
			i++;
		}
		attop.push_back(temp);
	}

	return attop;
}

vector <int> findmat(string matstr, vector<int> &att1, vector<string> &att2, vector<string> &att3, vector<string> &att4, int versize) {
	vector<string> attop = breakatt(matstr);
	vector<int> matu;

	for (int i = 0; i < versize; i++) {
		if (matchnum(att1[i], attop[0], attop[1]) && matchstr(att2[i], attop[2], attop[3]) && matchstr(att3[i], attop[4], attop[5]) && matchstr(att4[i], attop[6], attop[7])) {
			matu.push_back(i);
		}
	}

	return matu;
}

bool type1(int u, int v, string q, vector<vector<vector<int> > > &M) {
	if (M[u][v][q[0]-'a'] == 1) {
		return 1;
	}
	else {
		return 0;
	}
}


bool type2(int u, int v, string q, vector<vector<vector<int> > > &M) {
	int k = 0;
	int len = q.size();

	for (int i = 1; i < len; i++) {
		k = k * 10 + q[i] - '0';
	}

	if (M[u][v][q[0]-'a'] <= k) {
		return 1;
	}
	else {
		return 0;
	}

}

bool type3(int u, int v, string q, vector<vector<vector<int> > > &M) {
	if (M[u][v][q[0]-'a'] < INF && M[u][v][q[0]-'a'] != 0) {
		return 1;
	}
	else {
		return 0;
	}

}

bool query_result(int u, int v, string s, vector<vector<vector<int> > > &M, int versize) {
	vector<string> qs;
	int len = s.size();

	for (int i = 0; i < len;) {
		int j = i + 1;
		while (j < len && !isalpha(s[j])) {
			j++;
		}

		qs.push_back(s.substr(i, j-i));
		i = j;
	}

	vector<bool> curr_set(MAXVER, 0);
	vector<bool> next_set(MAXVER, 0);
	curr_set[u] = 1;
	len = qs.size();

	for (int i = 0; i < len; i++) {
		int qsize = qs[i].size();
		for (int j = 0; j < versize; j++) {
			if (curr_set[j] != 0) {
				if(qsize == 1) {
					for (int k = 0; k < versize; k++) {
						if (type1(j, k, qs[i], M)) {
							next_set[k] = 1;
						}
					}

				}
				else if(qs[i][1] != '+') {
					for (int k = 0; k < versize; k++) {
						if (type2(j, k, qs[i], M)) {
							next_set[k] = 1;
						}
					}
				}
				else {
					for (int k = 0; k < versize; k++) {
						if (type3(j, k, qs[i], M)) {
							next_set[k] = 1;
						}
					}
				}
			}
		}
		curr_set = next_set;
		fill(next_set.begin(), next_set.end(), 0);
	}

	return curr_set[v] == 1? true: false;
}

void matrix_build(vector<vector<vector<int> > > &adj, int versize, vector<vector<vector<int> > > &M) {
	for (int i = 0; i < versize; i++) {
		for (int col = 0; col < NCOLORS; col++) {
			int curr_vertex = i;
			queue<pair<int,int> > q;
			q.push(make_pair(curr_vertex, 0));
			vector<bool> flag(MAXVER, 0);
			bool cyclefound = false;
			while(!q.empty()){
				pair<int, int> v = q.front();
				q.pop();

				if (v.first != curr_vertex) {
					M[curr_vertex][v.first][col] = v.second;
				}
				for(vector<int>::iterator it = adj[v.first][col].begin(); it != adj[v.first][col].end(); ++it) {
					if (!cyclefound && *it == curr_vertex) {
						M[*it][*it][col] = v.second + 1;
						cyclefound = true;
						continue;
					}
					if (!flag[*it]) {
						q.push(make_pair(*it, v.second + 1));
						flag[*it] = true;
					}
				}
			}
		}
	}
	return;
}

int main() {
	freopen("../data/graph.txt","r",stdin);
	//freopen("data/output.txt","w",stdout);
	int versize, edgesize;

	cin >> versize >> edgesize;

	vector<int> att1(MAXVER); // Unique ID
	vector<string> att2(MAXVER); // Name
	vector<string> att3(MAXVER); // Country
	vector<string> att4(MAXVER); // Sex
	for (int i = 0; i < versize; i++) {
		cin >> att1[i] >> att2[i] >> att3[i] >> att4[i];
	}
	vector<vector<vector<int> > > adj(MAXVER,vector<vector<int> >(NCOLORS));
	int u, v;
	char color;
	for (int i = 0; i < edgesize; i++){
		cin >> u >> v >> color;
		adj[u - 1][color - 'a'].push_back(v - 1);
	}
	vector<int> M1(NCOLORS,INF);
	vector<vector<int> > M2(versize, M1);
	vector<vector<vector<int> > > M(versize, M2);

	matrix_build(adj, versize, M);
	string uatt, vatt, query;
	int querysize;
	int nq = 0, ni = 0;

	cin >> querysize;

	while(querysize--) {
		nq++;
		cin >> uatt >> vatt >> query;
        auto start = std::chrono::high_resolution_clock::now();
		vector<int> matu = findmat(uatt, att1, att2, att3, att4, versize);
		vector<int> matv = findmat(vatt, att1, att2, att3, att4, versize);
		int matusize = matu.size();
		int matvsize = matv.size();
		bool pairsfound = false;

		for (int i = 0; i < matusize; i++) {
			for (int j = 0; j < matvsize; j++) {
				if (query_result(matu[i], matv[j], query, M, versize)) {
					pairsfound = true;
					cout << "(" << matu[i] + 1 << "," << matv[j] + 1 << ") " << endl;
				}
			}
		}
		if (!pairsfound) {
			cout << "No pairs found.";
		}
        auto diff = std::chrono::high_resolution_clock::now() - start;
        auto t1 = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
        cout << "Time Taken: " << t1.count() << endl;
	}
	return 0;
}
