#include <iostream>
#include <vector>
#include <limits>
#include <chrono>
#include <cstdlib> 
#include <queue>
using namespace std;
//autor Tarik Velic
class Graph
{
    // Lista susjedstva gdje svaki par sadrži čvor susjeda i težinu grane
    vector<vector<pair<int, double>>> listaSusjedstva;
    int brojCvorova;

public:
    Graph(int V)
    {
        listaSusjedstva = vector<vector<pair<int, double>>>(V);
        brojCvorova = V;
    }

    void dodajGranu(int x, int y, double wt)
    {
        // Dodajemo granu od x do y s težinom wt
        listaSusjedstva[x].push_back(make_pair(y, wt));
        // Za neusmjereni graf, odkomentirajte sljedeću liniju
         listaSusjedstva[y].push_back(make_pair(x, wt));
    }

      void print()
    {
        for (int i = 0; i < brojCvorova; ++i)
        {
            cout << i << " -> ";
            for (int j = 0; j < listaSusjedstva[i].size(); ++j)
            {
                cout << "(" << listaSusjedstva[i][j].first << "," << listaSusjedstva[i][j].second << ") ";
            }
            cout << endl;
        }
    }

    void dijkstra(int izvor)
    {
        vector<int> udaljenost(brojCvorova, numeric_limits<int>::max());
        udaljenost[izvor] = 0;

        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
        pq.push({0, izvor});

        while (!pq.empty())
        {
            int trenutniCvor = pq.top().second;
            pq.pop();

            for (auto it = listaSusjedstva[trenutniCvor].begin(); it != listaSusjedstva[trenutniCvor].end(); ++it)
            {
                int susjedniCvor = it->first;
                int susjednaGrana = it->second;

                if (udaljenost[trenutniCvor] + susjednaGrana < udaljenost[susjedniCvor])
                {
                    udaljenost[susjedniCvor] = udaljenost[trenutniCvor] + susjednaGrana;
                    pq.push(make_pair(udaljenost[susjedniCvor], susjedniCvor));
                }
            }
        }

        // Za ispis udaljenosti od izvora do svakog čvora, odkomentirajte sljedeći blok
        
        for (int i = 0; i < brojCvorova; ++i) {
            cout << i << " je na udaljenosti " << udaljenost[i] << " od izvora" << endl;
        }
        
    }
};

int main()
{
    Graph g(4);
    g.dodajGranu(0, 1, 4);
    g.dodajGranu(0, 2, 1);
    g.dodajGranu(2, 1, 2);
    g.dodajGranu(1, 3, 1);
    g.dodajGranu(2, 3, 7);
    g.print();
    g.dijkstra(0);

    return 0;
}
