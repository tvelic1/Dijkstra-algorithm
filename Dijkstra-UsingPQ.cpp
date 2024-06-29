#include <iostream>
#include <vector>
#include <limits>
#include <chrono>
#include <cstdlib> // Za rand() i srand()
#include <queue>
using namespace std;

template<typename T, int V>
class Graph {
    // Lista susjedstva gdje svaki par sadrži čvor susjeda i težinu grane
    vector<vector<pair<T, int>>> adjList;

public:
    Graph() : adjList(V) {}

    void addEdge(T x, T y, int wt) {
        // Dodajemo granu od x do y s težinom wt
        adjList[x].push_back(make_pair(y, wt));
        // Za neusmjereni graf, odkomentirajte sljedeću liniju
        // adjList[y].push_back(make_pair(x, wt));
    }

    void print() {
        for (int i = 0; i < V; ++i) {
            cout << i << " -> ";
            for (auto &p : adjList[i]) {
                cout << "(" << p.first << "," << p.second << ") ";
            }
            cout << endl;
        }
    }

    void dijkstraSSSP(T src) {
        vector<int> dist(V, numeric_limits<int>::max());
        dist[src] = 0;

        priority_queue<pair<int, T>, vector<pair<int, T>>, greater<pair<int, T>>> pq;
        pq.push({0, src});

        while (!pq.empty()) {
            T currNode = pq.top().second;
            pq.pop();

            for (auto nbr : adjList[currNode]) {
                T nbrNode = nbr.first;
                int edgeDist = nbr.second;

                if (dist[currNode] + edgeDist < dist[nbrNode]) {
                    dist[nbrNode] = dist[currNode] + edgeDist;
                    pq.push(make_pair(dist[nbrNode], nbrNode));
                }
            }
        }

        // Za ispis udaljenosti od izvora do svakog čvora, odkomentirajte sljedeći blok
        /*
        for (int i = 0; i < V; ++i) {
            cout << i << " is at distance " << dist[i] << " from source" << endl;
        }
        */
    }
};

int main() {
    srand(time(0)); // Inicijalizacija generatora slučajnih brojeva

    const int V = 1000;  // Broj vrhova u grafu
    Graph<int,V> g;
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < 1000; ++j) {
            if (i != j ) { 
                int slucajniBroj = rand() % 100; // Generiše broj između 0 i 99
                if (slucajniBroj < 70) { // 70% šansa
                    int weight = 5 + rand() % 99; // Generiše težinu između 5 i 99
                    g.addEdge(i, j, weight);
                }
            }
        }
    }

    //g.print();
    cout << endl;
    auto start = std::chrono::high_resolution_clock::now();

    g.dijkstraSSSP(0);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    cout << "Vrijeme izvršavanja: " << duration.count() << " milisekundi" << endl;

    return 0;
}

