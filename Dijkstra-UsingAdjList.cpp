#include <iostream>
#include <map>
#include <list>
#include <set>
#include <climits>
#include <chrono>
#include <cstdlib>
#include <tuple>
#include <vector>
//author Tarik Velic
using namespace std;

class Graph
{
    map<int, list<tuple<int, double>>> l;

public:
    void dodajGranu(int x, int y, double wt)
    {
        l[x].push_back(make_tuple(y, wt));
        if (l.find(y) == l.end())
        {
            l[y] = list<tuple<int, double>>();
        }
    }

    void print()
    {
        for (auto it = l.begin(); it != l.end(); ++it)
        {
            int cvor = it->first;
            cout << cvor << " -> ";
            for (auto susjedIt = it->second.begin(); susjedIt != it->second.end(); ++susjedIt)
            {
                int susjedniCvor;
                double susjednaGrana;
                tie(susjedniCvor, susjednaGrana) = *susjedIt;
                cout << "(" << susjedniCvor << "," << susjednaGrana << ") ";
            }
            cout << endl;
        }
    }

    void dijkstra(int src)
    {
        int V = l.size();
        vector<double> udaljenost(V, numeric_limits<double>::max());
        vector<bool> posjecen(V, false);

        udaljenost[src] = 0;

        for (int i = 0; i < V - 1; ++i)
        {
            int u = minUdaljenost(udaljenost, posjecen);
            posjecen[u] = true;

            for (const auto &susjed : l[u])
            {
                int v = get<0>(susjed);
                double weight = get<1>(susjed);

                if (!posjecen[v] && udaljenost[u] != numeric_limits<double>::max() &&
                    udaljenost[u] + weight < udaljenost[v])
                {
                    udaljenost[v] = udaljenost[u] + weight;
                }
            }
        }

        /*for (int i = 0; i < V; ++i)
        {
            cout << "Node " << i << " is at udaljenostance " << udaljenost[i] << " from source" << endl;
        }*/
    }

    // Pomoćna funkcija za pronalaženje čvora s najmanjom udaljenošću
    int minUdaljenost(const vector<double> &udaljenost, const vector<bool> &posjecen)
    {
        double min = numeric_limits<double>::max();
        int min_index = -1;

        for (int v = 0; v < udaljenost.size(); ++v)
        {
            if (!posjecen[v] && udaljenost[v] <= min)
            {
                min = udaljenost[v];
                min_index = v;
            }
        }

        return min_index;
    }
};

int main()
{
    srand(time(0)); 

    Graph g;
    for (int i = 0; i < 1000; ++i)
    {
        for (int j = 0; j < 1000; ++j)
        {
            if (i != j)
            {
                if (rand() % 100 < 90)
                {
                    double weight = 5 + rand() % 99;
                    g.dodajGranu(i, j, weight);
                }
            }
        }
    }

    cout << endl;
    auto start = std::chrono::high_resolution_clock::now();

    g.dijkstra(0);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    cout << "Vrijeme izvršavanja: " << duration.count() << " miliseconds" << endl;

    return 0;
}
