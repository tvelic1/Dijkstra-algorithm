#include <iostream>
#include <map>
#include <list>
#include <set>
#include <climits> 
#include <time.h>
#include <chrono>
#include <cstdlib> 

using namespace std;

template <typename T> 
class Graph
{								  
	map<T, list<pair<T, int>>> l; 

public: 
	void addEdge(T x, T y, int wt)
	{ 
		l[x].push_back({y, wt});
		//l[y].push_back({x, wt}); // to make the graph unidirectional just remove this line
	}

	/*void print() {
		for (auto p : l) {
			T node = p.first;
			cout << node << " -> ";

			for (auto nbr : l[node]) {
				cout << "(" << nbr.first << "," << nbr.second << ") ";
			} cout << endl;
		}
	}*/

	void print()
	{
		for (auto it = l.begin(); it != l.end(); ++it)
		{
			T node = it->first;
			cout << node << " -> ";
			for (auto nbrIt = l[node].begin(); nbrIt != l[node].end(); ++nbrIt)
			{
				cout << "(" << nbrIt->first << "," << nbrIt->second << ") ";
			}
			cout << endl;
		}
	}

	void djikstraSSSP(T src)
	{

		map<T, int> dist;

		for (auto p : l)
		{
			T node = p.first;
			dist[node] = INT_MAX;
		}
		dist[src] = 0;

		set<pair<int, T>> s;
		s.insert({dist[src], src});

		while (!s.empty())
		{

			pair<int, T> p = *s.begin(); 
			s.erase(s.begin());
			T currNode = p.second;
			int currNodeDist = p.first;

			for (auto nbr : l[currNode])
			{ 
				T nbrNode = nbr.first;
				int distInBetween = nbr.second;
				int nbrNodeDist = dist[nbrNode];
				if (currNodeDist + distInBetween < nbrNodeDist)
				{

					auto pr = s.find({dist[nbrNode], nbrNode});
					if (pr != s.end())
					{
						s.erase(pr);
					}
					dist[nbrNode] = currNodeDist + distInBetween;
					s.insert({dist[nbrNode], nbrNode});
				}
			}
		}

		/*for (auto x : dist) {
			cout << x.first << " is at distance " << x.second << " from source" << endl;
		}*/
		/*for (auto it = dist.begin(); it != dist.end(); ++it)
		{
			cout << it->first << " is at distance " << it->second << " from source" << endl;
		}*/
	}
};

int main()
{

	srand(time(0)); // Inicijalizacija generatora slučajnih brojeva

	Graph<int> g; // Sada kreiramo instancu Graph klase koja radi sa int tipom
	for (int i = 0; i < 1000; ++i)
	{
		for (int j = 0; j < 1000; ++j)
		{
			if (i != j)
			{ 
				if (rand() % 100 < 30)
				{ // 30% šansa da se stvori grana
					 int weight = 5 + rand() % 99; // Generiše težinu između 5 i 100
					g.addEdge(i, j, weight);
				}
			}
		}
	}

	//g.print();
	cout << endl;
	auto start = std::chrono::high_resolution_clock::now();

	g.djikstraSSSP(0);

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Vrijeme izvršavanja: " << duration.count() << " miliseconds" << std::endl;
   // g.print();
	cout << endl;
}
