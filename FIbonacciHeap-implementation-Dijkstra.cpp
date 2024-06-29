#include <iostream>
#include <string>
#include <climits>
#include <algorithm>
#include <vector>
#include <tuple>
#include <set>
#include <time.h>
#include <chrono>
#include <cstdlib> // Za rand() i srand()

//source : https://stackoverflow.com/questions/14333314/why-is-a-fibonacci-heap-called-a-fibonacci-heap?fbclid=IwZXh0bgNhZW0CMTAAAR1imm4yszZNb6Qn4bJSEYhb9GNib7tA7phTjqE8bqJZaYzwS4jAUkoKd34_aem_K-B1EAlRSA7igJk9_0ryeQ

using namespace std;

const int infty = INT_MAX;

// Code for printing out a vector
template <typename T>
ostream &operator<<(ostream &s, vector<T> t)
{
    s << "[";
    for (size_t i = 0; i < t.size(); i++)
    {
        s << t[i] << (i == t.size() - 1 ? "" : ",");
    }
    return s << "] ";
}

// Structs used with the adjacency list.
struct Neighbour
{
    int vertex; // Index of neighbouring vertex
    int weight; // weight of the associated arc
};

// Graph Class (uses an adjacency list)
class Graph
{
public:
    int n; // Num vertices
    int m; // Num arcs
    vector<vector<Neighbour>> adj;
    Graph(int n)
    {
        this->n = n;
        this->m = 0;
        this->adj.resize(n, vector<Neighbour>());
    }
    ~Graph()
    {
        this->n = 0;
        this->m = 0;
        this->adj.clear();
    }
    void addArc(int u, int v, int w)
    {
        this->adj[u].push_back(Neighbour{v, w});
        this->m++;
    }
};

// Struct and ordering/comparison operator used with self-balancing tree (c++ set)
struct HeapItem
{
    int vertex;
    int label;
};
struct minHeapItem
{
    bool operator()(const HeapItem &lhs, const HeapItem &rhs) const
    {
        return tie(lhs.label, lhs.vertex) < tie(rhs.label, rhs.vertex);
    }
};

// Struct for a Fibonacci heap node
struct FibonacciNode
{
    int degree;            // Number of children.
    FibonacciNode *parent; // Pointer to parent
    FibonacciNode *child;  // Pointer to first child
    FibonacciNode *left;   // Pointer to left sibling.
    FibonacciNode *right;  // Pointer to right sibling.
    bool mark;             // Is the node marked?
    int key;               // Node's key value.
    int nodeIndex;         // The nodes index value (referring to the vertex in the problem graph)
};

// Fibonacci heap class
class FibonacciHeap
{
private:
    FibonacciNode *minNode;
    int numNodes;
    vector<FibonacciNode *> degTable;
    vector<FibonacciNode *> nodePtrs;

public:
    FibonacciHeap(int n)
    {
        // Constructor function
        this->numNodes = 0;
        this->minNode = NULL;
        this->degTable = {};
        this->nodePtrs.resize(n);
    }
    ~FibonacciHeap()
    {
        // Destructor function
        this->numNodes = 0;
        this->minNode = NULL;
        this->degTable.clear();
        this->nodePtrs.clear();
    }
    int size()
    {
        // Number of nodes in the heap
        return this->numNodes;
    }
    bool empty()
    {
        // Is the heap empty?
        if (this->numNodes > 0)
            return false;
        else
            return true;
    }
    void insert(int u, int key)
    {
        // Insert the vertex u with the specified key (value for L(u)) into the Fibonacci heap. O(1) operation
        this->nodePtrs[u] = new FibonacciNode;
        this->nodePtrs[u]->nodeIndex = u;
        FibonacciNode *node = this->nodePtrs[u];
        node->key = key;
        node->degree = 0;
        node->parent = NULL;
        node->child = NULL;
        node->left = node;
        node->right = node;
        node->mark = false;
        FibonacciNode *minN = this->minNode;
        if (minN != NULL)
        {
            FibonacciNode *minLeft = minN->left;
            minN->left = node;
            node->right = minN;
            node->left = minLeft;
            minLeft->right = node;
        }
        if (minN == NULL || minN->key > node->key)
        {
            this->minNode = node;
        }
        this->numNodes++;
    }
    FibonacciNode *extractMin()
    {
        // Extract the node with the minimum key from the heap. O(log n) operation, where n is the number of nodes in the heap
        FibonacciNode *minN = this->minNode;
        if (minN != NULL)
        {
            int deg = minN->degree;
            FibonacciNode *currChild = minN->child;
            FibonacciNode *remChild;
            for (int i = 0; i < deg; i++)
            {
                remChild = currChild;
                currChild = currChild->right;
                _existingToRoot(remChild);
            }
            _removeNodeFromRoot(minN);
            this->numNodes--;
            if (this->numNodes == 0)
            {
                this->minNode = NULL;
            }
            else
            {
                this->minNode = minN->right;
                FibonacciNode *minNLeft = minN->left;
                this->minNode->left = minNLeft;
                minNLeft->right = this->minNode;
                _consolidate();
            }
        }
        return minN;
    }
    void decreaseKey(int u, int newKey)
    {
        // Decrease the key of the node in the Fibonacci heap that has index u. O(1) operation
        FibonacciNode *node = this->nodePtrs[u];
        if (newKey > node->key)
            return;
        node->key = newKey;
        if (node->parent != NULL)
        {
            if (node->key < node->parent->key)
            {
                FibonacciNode *parentNode = node->parent;
                _cut(node);
                _cascadingCut(parentNode);
            }
        }
        if (node->key < this->minNode->key)
        {
            this->minNode = node;
        }
    }

private:
    // The following are private functions used by the public methods above
    void _existingToRoot(FibonacciNode *newNode)
    {
        FibonacciNode *minN = this->minNode;
        newNode->parent = NULL;
        newNode->mark = false;
        if (minN != NULL)
        {
            FibonacciNode *minLeft = minN->left;
            minN->left = newNode;
            newNode->right = minN;
            newNode->left = minLeft;
            minLeft->right = newNode;
            if (minN->key > newNode->key)
            {
                this->minNode = newNode;
            }
        }
        else
        {
            this->minNode = newNode;
            newNode->right = newNode;
            newNode->left = newNode;
        }
    }
    void _removeNodeFromRoot(FibonacciNode *node)
    {
        if (node->right != node)
        {
            node->right->left = node->left;
            node->left->right = node->right;
        }
        if (node->parent != NULL)
        {
            if (node->parent->degree == 1)
            {
                node->parent->child = NULL;
            }
            else
            {
                node->parent->child = node->right;
            }
            node->parent->degree--;
        }
    }
    void _cut(FibonacciNode *node)
    {
        _removeNodeFromRoot(node);
        _existingToRoot(node);
    }
    void _addChild(FibonacciNode *parentNode, FibonacciNode *newChildNode)
    {
        if (parentNode->degree == 0)
        {
            parentNode->child = newChildNode;
            newChildNode->right = newChildNode;
            newChildNode->left = newChildNode;
            newChildNode->parent = parentNode;
        }
        else
        {
            FibonacciNode *child1 = parentNode->child;
            FibonacciNode *child1Left = child1->left;
            child1->left = newChildNode;
            newChildNode->right = child1;
            newChildNode->left = child1Left;
            child1Left->right = newChildNode;
        }
        newChildNode->parent = parentNode;
        parentNode->degree++;
    }
    void _cascadingCut(FibonacciNode *node)
    {
        FibonacciNode *parentNode = node->parent;
        if (parentNode != NULL)
        {
            if (node->mark == false)
            {
                node->mark = true;
            }
            else
            {
                _cut(node);
                _cascadingCut(parentNode);
            }
        }
    }
    void _link(FibonacciNode *highNode, FibonacciNode *lowNode)
    {
        _removeNodeFromRoot(highNode);
        _addChild(lowNode, highNode);
        highNode->mark = false;
    }
    void _consolidate()
    {
        int deg, rootCnt = 0;
        if (this->numNodes > 1)
        {
            this->degTable.clear();
            FibonacciNode *currNode = this->minNode;
            FibonacciNode *currDeg, *currConsolNode;
            FibonacciNode *temp = this->minNode, *itNode = this->minNode;
            do
            {
                rootCnt++;
                itNode = itNode->right;
            } while (itNode != temp);
            for (int cnt = 0; cnt < rootCnt; cnt++)
            {
                currConsolNode = currNode;
                currNode = currNode->right;
                deg = currConsolNode->degree;
                while (true)
                {
                    while (deg >= int(this->degTable.size()))
                    {
                        this->degTable.push_back(NULL);
                    }
                    if (this->degTable[deg] == NULL)
                    {
                        this->degTable[deg] = currConsolNode;
                        break;
                    }
                    else
                    {
                        currDeg = this->degTable[deg];
                        if (currConsolNode->key > currDeg->key)
                        {
                            swap(currConsolNode, currDeg);
                        }
                        if (currDeg == currConsolNode)
                            break;
                        _link(currDeg, currConsolNode);
                        this->degTable[deg] = NULL;
                        deg++;
                    }
                }
            }
            this->minNode = NULL;
            for (size_t i = 0; i < this->degTable.size(); i++)
            {
                if (this->degTable[i] != NULL)
                {
                    _existingToRoot(this->degTable[i]);
                }
            }
        }
    }
};
// End of FibonacciHeap class

tuple<vector<int>, vector<int>> dijkstraFibanocci(Graph &G, int s)
{
    int u, v, w;
    FibonacciHeap Q(G.n);
    vector<int> L(G.n), P(G.n);
    vector<bool> D(G.n);
    // Initialise the data structures
    for (int u = 0; u < G.n; u++)
    {
        D[u] = false;
        L[u] = infty;
        P[u] = -1;
    }
    // Main Dijkstra algorithm
    L[s] = 0;
    Q.insert(s, 0);
    while (!Q.empty())
    {
        u = Q.extractMin()->nodeIndex;
        D[u] = true;
        for (auto &neighbour : G.adj[u])
        {
            v = neighbour.vertex;
            w = neighbour.weight;
            if (D[v] == false)
            {
                if (L[u] + w < L[v])
                {
                    if (L[v] == infty)
                    {
                        Q.insert(v, L[u] + w);
                    }
                    else
                    {
                        Q.decreaseKey(v, L[u] + w);
                    }
                    L[v] = L[u] + w;
                    P[v] = u;
                }
            }
        }
    }
    return make_tuple(L, P);
}

tuple<vector<int>, vector<int>> dijkstraSelfBalancingTree(Graph &G, int s)
{
    int u, v, w;
    set<HeapItem, minHeapItem> Q;
    vector<int> L(G.n), P(G.n);
    vector<bool> D(G.n);
    // Initialise the data structures
    for (u = 0; u < G.n; u++)
    {
        D[u] = false;
        L[u] = infty;
        P[u] = -1;
    }
    // Main Dijkstra algorithm
    L[s] = 0;
    Q.emplace(HeapItem{s, 0});
    while (!Q.empty())
    {
        u = (*Q.begin()).vertex;
        Q.erase(*Q.begin());
        D[u] = true;
        for (auto &neighbour : G.adj[u])
        {
            v = neighbour.vertex;
            w = neighbour.weight;
            if (D[v] == false)
            {
                if (L[u] + w < L[v])
                {
                    if (L[v] == infty)
                    {
                        Q.emplace(HeapItem{v, L[u] + w});
                    }
                    else
                    {
                        Q.erase({v, L[v]});
                        Q.emplace(HeapItem{v, L[u] + w});
                    }
                    L[v] = L[u] + w;
                    P[v] = u;
                }
            }
        }
    }
    return make_tuple(L, P);
}

vector<int> getPath(int u, int v, vector<int> &P)
{
    // Get the u-v-path specified by the P vector
    vector<int> path;
    int x = v;
    if (P[x] == -1)
        return path;
    while (x != u)
    {
        path.push_back(x);
        x = P[x];
    }
    path.push_back(u);
    reverse(path.begin(), path.end());
    return path;
}

int main()
{
    // Construct an example graph (a directed cycle on 5 vertices here)
    Graph G(10000); // Kreira graf sa 8 čvorova
    vector<vector<bool>> vec(10000, vector<bool>(10000, false));
    srand(time(0)); // Inicijalizacija generatora slučajnih brojeva

    for (int u = 0; u < 1000; ++u)
    {
        for (int v = 0; v < 1000; ++v)
        {
            if (u != v && !vec[u][v] )
            {                                    // Osiguravamo da u i v nisu isti
                int slucajniBroj = rand() % 100; // Generiše broj između 0 i 99
                if (slucajniBroj < 70)
                {                                  // 90% šansa
                    int weight = 5 + rand() % 99; // Generiše težinu između 5 i 1000
                    G.addArc(u, v, weight);
                    G.addArc(v, u, weight);
                    vec[v][u] = true;
                }
            }
        }
    }
    // Set the source vertex
    int s = 0;

    // Declare some variables
    vector<int> L, P;

    // Execute Dijkstra's algorithm using a Fibonacci heap
    auto start = std::chrono::high_resolution_clock::now();
    tie(L, P) = dijkstraFibanocci(G, s);
    // tie(L, P) = dijkstraSelfBalancingTree(G, s);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Vrijeme izvršavanja: " << duration.count() << " miliseconds" << std::endl;
    /*tie(L, P) = dijkstraSelfBalancingTree(G, s);


//Execute Dijkstra's algorithm using a self-balancing tree

//Output some information
cout << "Input graph has " << G.n << " vertices and " << G.m << " arcs\n";
/* cout << "Dijkstra with Fibonacci heap took " << duration1 << " seconds\n";
cout << "Dijkstra with self-balancing tree took " << duration2 << " seconds\n";*/
    /**/ // cout << "Shortest paths from source to each vertex are as follows:\n";
    /* for (int u = 0; u < G.n; u++) {
         cout << "v-" << s << " to v-" << u << ",\t";
         if (L[u] == infty)
             cout << "length = infinity. No path exists\n";
         else
             cout << "length = " << L[u] << ",\tpath = " << getPath(s, u, P) << "\n";
     }*/
}