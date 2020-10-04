// defined std::unique_ptr
#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"
// defined std::cout#include <iostream>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string.h>
#include <queue>
#include<algorithm>

using namespace std;
using namespace Minisat;

#define MAX 100000

#define INFINITY 200000


struct link
{
	int data;
	link* next;
};
struct Node
{
	int v;
	link* next;
};

typedef struct {
	Node node[MAX + 1];
	int nodeCnt;
}Graph;

Graph g;

int visited[MAX + 1];
int pa[MAX + 1];

Graph readGraph(vector<int> vec_V, vector<int>vec_E)
{
	//ifstream cin("D://UW//ece650//a2//x22yi//1.txt");
	int num_E = vec_E.size();
	int num_V = vec_E.size();
	Graph graph;
	int i;
	for (i = 0; i < MAX; i++)
	{
		graph.node[i].v = i;
		graph.node[i].next = NULL;
	}

	int n1 = 0, n2 = 0;
	link* s;
	graph.nodeCnt = 1;
	for (int i = 1; i <= num_E; i++) {
		n1 = vec_E[i];
		n2 = vec_E[i - 1];
		if (n2 == 0) {
			int n0 = n1;
			n1 = n2;
			n2 = n0;
		}

		if (graph.nodeCnt < n1) graph.nodeCnt = n1;
		if (graph.nodeCnt < n2) graph.nodeCnt = n2;
		s = new link;
		s->data = n2;
		s->next = graph.node[n1].next;
		graph.node[n1].next = s;
		//delete(s);   

		s = new link;
		s->data = n1;
		s->next = graph.node[n2].next;
		graph.node[n2].next = s;
		//delete(s);  
		i++;
	}
	return graph;
}

void shortestPath(Graph graph, int s, int d) // Using BFS. REFER TO :http://lingyibin.javaeye.com/blog/849828
{

	queue<int> que;
	link* p = NULL;
	//int cnt = 0;   
	int parents = s;
	memset(visited, 0, sizeof(visited));
	memset(pa, 0, sizeof(pa));
	visited[s] = 1;
	pa[s] = -1;
	que.push(s);
	while (!que.empty()) {
		p = graph.node[que.front()].next;
		parents = que.front();
		que.pop();
		//cnt ++;   
		while (p != NULL)
		{
			if (!visited[p->data])
			{
				visited[p->data] = 1;
				pa[p->data] = parents;

				if (p->data == d)
				{

					break;
				}
				que.push(p->data);
			}
			p = p->next;
		}
	}

	parents = d;
	//cout<<parents<<" <- ";   
	vector <int> printlist;

	while (pa[parents] != -1)
	{
		//cout << parents << " - ";
		printlist.push_back(parents);

		parents = pa[parents];
	}
	//cout << parents << endl;
	printlist.push_back(parents);

	if (!printlist.empty()) {
		int m = printlist.size();
		for (int i = 1; i < m; i++) {
			cout << printlist[m - i];
			cout << "-";
		}
		cout << printlist[0];
		cout << endl;
	}
	else {
		cout << "Error: no shortest path."<<endl;
	}

}
bool is_element_in_vector(vector<int> v, int element) {
	vector<int>::iterator it;

	it = find(v.begin(), v.end(), element);
	if (it != v.end()) {
		return true;
	}

	else {
		return false;
	}
}


bool binary_vertexcover(int vertex_num, vector<int>edge, std::vector<int>& vertex_cover) { //REF https://blog.csdn.net/qq_29848559/article/details/86671904 (4)Minisat
	Solver solver;

	vector<int> result;

	int hi = vertex_num;
	int low = 1;
	int mid;

	while (hi >= low) {
		int mid = (hi + low) / 2;
		//

		if (not find_vertexcover(vertex_num, edge, mid, vertex_cover)) {
			hi = mid - 1;
			result = vertex_cover;
		}
		else {
			low = mid + 1;
		}

	}
}

bool find_vertexcover(int vertex_num, int k, vector<int>edge, std::vector<int>& vertex_cover) {

	Solver solver;

	Var propositions[vertex_num][k];

	//vector<vector<Minisat::Lit> > vec;

	for (int i = 0; i < vertex_num; i++) {
		for (int j = 0; j < k; j++) {
			propositions[i][j] = solver.newVar();
		}
	}

	//std::vector<std::vector<Minisat::Lit> > x(vertex_num);

	// clause1: 

	for (int i = 0; i < k; i++) {
		vec<Lit> clause1;
		for (int j = 0; j < vertex_num; j++) {
			clause1.push(mkLit(propositions[j][i]));
		}
		solver.addClause(clause1);
		clause1.clear();

	}

	// clause 2. No one vertex can appear twice in a vertex cover

	for (int m = 0; m < vertex_num; m++) {
		for (int p = 0; p < k - 1; p++) {
			for (int q = p + 1; q < k; q++) {
				solver.addClause(mkLit(propositions[m][p], true), mkLit(propositions[m][q], true));
			}
		}
	}

	// clause 3. No more than one vertex appears in the mth position of the vertex cover.

	for (int m = 0; m < k; m++)

	{

		for (int p = 0; p < vertex_num - 1; p++) {

			for (int q = p + 1; q < vertex_num; q++) {
				solver.addClause(mkLit(propositions[p][m], true), mkLit(propositions[q][m], true));
			}
		}
	}

	// 4. Every edge is incident to at least one vertex in the vertex cover
	for (int i = 0; i < edge.size(); i++)
	{
		vec<Lit> clause4;
		for (int m = 0; m < k; m++)

		{

			clause4.push(mkLit(propositions[edge[i]][m]));

			clause4.push(mkLit(propositions[edge[i + 1]][m]));

		}
		solver.addClause(clause4);
		clause4.clear();
	}

	vector<int> vertex_cover;
	if (solver.solve()) {
		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < vertex_num; j++)
			{

				if (toInt(solver.modelValue(propositions[j][i])) == l_True)

				{
					vertex_cover.push_back(i);
					break;
				}

			}
		}
		return 1;
		//sort
		sort(vertex_cover.begin(), vertex_cover.end());
	}
	//sort
	return 0;
}


int main(int argc, char** argv) {
	vector<int> vec_V;
	vector<int> vec_E;


	// read from stdin until EOF
	while (!::cin.eof()) {
		
		string line;
		//getline(cin, line);

		//istringstream input(line);

		while (getline(cin, line)) {
			char arr[MAX];
			int vertex[MAX];

			strcpy(arr, line.c_str());

			if (arr[0] == 'V') {
				//cout << "vertex\n";
				cout << line << endl; //a3: edit it to print the graph that it has read before accepting an s command.

				int num = 0;
				for (int i = 0; arr[i] != '\0'; ++i) {

					if (arr[i] >= '0' && arr[i] <= '9')

						num = num * 10 + arr[i] - '0';
				}


				for (int j = 0; j < num; ++j) {
					vec_V.push_back(j);
				}
				//for (vector<int>::iterator it = vec_V.begin(); it != vec_V.end(); it++) {
				//	//cout << *it << " ";
				//}//print vector
				while (vec_V.empty()) {
					//cout << "Error: NO vertex is enterd." << endl;
					break;
				}
				break;
			}

			else if (arr[0] == 'E') {
				int num = 0;
				int d_edge = 0;
				cout << line << endl;//a3: edit it to print the graph that it has read before accepting an s command.
				for (int i = 0; arr[i] != '\0'; i++) {

					if (arr[i] > '0' && arr[i] <= '9') {
						/*cout << arr[i] << "\n";*/
						num = num * 10 + arr[i] - '0';
					}
					else {
						if (arr[i] == '0' && num == 0) {
							vec_E.push_back(0);
						}
						if (arr[i] == '0' && (num != 0)) {
							num = num * 10 + arr[i] - '0';
							vec_E.push_back(num);
							num = 0;
						}
						if (arr[i] != '0' && (num != 0)) {
							vec_E.push_back(num);
							num = 0;
						}

					}
				}
				for (int i = 0; i < vec_E.size(); i++) {

					if (is_element_in_vector(vec_V, vec_E[i]) == false) {
						//cout << vec_E[i];
						cout << "Error:some vertex in  edge not exists.";// check the error of edge: not a vertex
						vec_E.erase(vec_E.begin() + i);
						return 0;
					}
				}
				while (vec_E.empty()) {
					cout << "Error: NO edge is enterd." << endl;
					return 0;
				}
				break;
			}

			else if (arr[0] == 's') {
				vector<int> vec_s;

				Graph graph = readGraph(vec_V, vec_E);

				vector<char> new_vec;
				char new_arr[MAX];

				for (int a = 0; arr[a] != '\0'; a++) {
					new_vec.push_back(arr[a]);
				}

				new_vec.push_back('.');

				/*for (int i = 0; i < new_vec.size(); i++) {
					cout << new_vec[i] ;
				}*/
				for (int i = 0; i < new_vec.size(); i++) {
					new_arr[i] = new_vec[i];
				}
				int num = 0;

				for (int i = 0; new_arr[i] != '\0'; i++) {

					if (new_arr[i] > '0' && new_arr[i] <= '9') {
						/*cout << arr[i] << "\n";*/
						num = num * 10 + new_arr[i] - '0';
					}
					else {
						if (new_arr[i] == '0' && num == 0) {
							vec_s.push_back(0);
						}
						if (new_arr[i] == '0' && (num != 0)) {
							num = num * 10 + new_arr[i] - '0';
							vec_s.push_back(num);
							num = 0;
						}
						if (new_arr[i] != '0' && (num != 0)) {
							vec_s.push_back(num);
							num = 0;
						}

					}
				}

				while (vec_s.empty() || (is_element_in_vector(vec_V, vec_s[0]) == false) || (is_element_in_vector(vec_E, vec_s[1]) == false || vec_s[0] == vec_s[1])) {
					cout << "Error: Wrong endpoints." << endl; //check the error of enpoints input:1. not entered/2. not in an edge/3. start and end are the same.
					return 0;
				}
				//for (int i = 0; i < vec_s.size(); i++) {

				//	if (is_element_in_vector(vec_V, vec_s[i]) == false) {
				//		//cout << vec_E[i];
				//		cout << "Error: vertex not exists.";
				//		vec_E.erase(vec_E.begin() + i);
				//		return 0;
				//	}


				//}
				int v0 = vec_s[0];
				int v1 = vec_s[1];
				//for (vector<int>::iterator it = vec_s.begin(); it != vec_s.end(); it++) {
				//	cout << *it << " ";
				//}//print vector
				shortestPath(graph, v0, v1);

				//break;
			}
		}

	}
	return 0;
}
