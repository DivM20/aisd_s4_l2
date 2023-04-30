#include <iostream>
#include <stdlib.h>
#include <climits>
#include <ctime>
#include <chrono>
#include <queue>
#include <fstream>


using namespace std;

//базовые функции

void array_out(int** arr, int V) {
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
}

void dist_out(int* dist,int V) {
    for (int i = 0; i < V; i++) {
        cout << dist[i] << " ";
    }
    cout << endl;
}

int** standart_fill(int el_num, int connectivity = 0) {
    int** grapgh_matr = new int* [el_num];
    for (int i = 0; i < el_num; i++) {
        grapgh_matr[i] = new int[el_num];
    }

    for (int i = 0; i < el_num; i++) {
        for (int j = 0; j < el_num; j++) {
            grapgh_matr[i][j] = 0;
        }
    }


    //генерация граней
    int edges_num = 0;

    switch (connectivity) {
    case 0:
        edges_num = el_num + rand() % (int((el_num * (el_num - 1)) / 4) - el_num + 1); break;
    case 1:
        edges_num = el_num + rand() % (int((el_num * (el_num - 1)) / 2) - el_num + 1); break;
    case 2:
        edges_num = ((el_num * (el_num - 1)) / 2); break;
    default:break;
    }


    int e = 0;
    int v1 = 0, v2 = 0;
    while (e < edges_num)
    {
        while (v1 == v2) {
            v1 = rand() % el_num; v2 = rand() % el_num;
            if (grapgh_matr[v1][v2] != 0) {
                v1 = 0; v2 = 0;
            }
        }
        grapgh_matr[v1][v2] = 1 + (rand() % 50); grapgh_matr[v2][v1] = grapgh_matr[v1][v2];
        e += 1; v1 = 0; v2 = 0;
    }

    bool connected = false; v2 = 0;
    for (int i = 0; i < el_num; i++) {
        connected = false; v2 = i;
        for (int j = 0; j < el_num; j++) {
            if (grapgh_matr[i][j] != 0) { connected = true; break; }
        }
        if (connected == false) {
            while (v2 == i) { v2 = rand() % el_num; }
            grapgh_matr[i][v2] = 1 + (rand() % 50); grapgh_matr[v2][i] = grapgh_matr[i][v2];
        }
    }

    return grapgh_matr;
}

//стандартная реализация

int minDistance(int dist[], bool used[],int V)
{

    // Initialize min value
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++)
        if (used[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

int* standart_dijkstra(int** matrix, int V,int S){
    
    int* dist = new int[V];
    bool* used = new bool[V]; 

    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, used[i] = false;

    dist[S] = 0;

    for (int count = 0; count < V - 1; count++) {

        int u = minDistance(dist, used,V);

        used[u] = true;

        for (int v = 0; v < V; v++) {
            if (!used[v] && matrix[u][v] && dist[u] != INT_MAX && dist[u] + matrix[u][v] < dist[v]) {
                dist[v] = dist[u] + matrix[u][v];
            }
        }
    }

    return dist;
}

namespace BinHeap {
    const int max_n = 10000;
    const int max_e = 1000000;


    int h[max_n];
    int pos2Id[max_n];
    int id2Pos[max_n];
    int hsize;

    void hswap(int i, int j) {
        std::swap(h[i], h[j]);
        std::swap(pos2Id[i], pos2Id[j]);
        std::swap(id2Pos[pos2Id[i]], id2Pos[pos2Id[j]]);
    }

    void moveUp(int pos) {
        while (pos > 0) {
            int parent = (pos - 1) >> 1;
            if (h[pos] >= h[parent]) {
                break;
            }
            hswap(pos, parent);
            pos = parent;
        }
    }

    void add(int id, int prio) {
        h[hsize] = prio;
        pos2Id[hsize] = id;
        id2Pos[id] = hsize;
        moveUp(hsize++);
    }

    void increasePriority(int id, int prio) {
        int pos = id2Pos[id];
        h[pos] = prio;
        moveUp(pos);
    }

    void moveDown(int pos) {
        while (pos < (hsize >> 1)) {
            int child = 2 * pos + 1;
            if (child + 1 < hsize && h[child + 1] < h[child]) {
                ++child;
            }
            if (h[pos] <= h[child]) {
                break;
            }
            hswap(pos, child);
            pos = child;
        }
    }

    int removeMin() {
        int res = pos2Id[0];
        int lastNode = h[--hsize];
        if (hsize > 0) {
            h[0] = lastNode;
            int id = pos2Id[hsize];
            id2Pos[id] = 0;
            pos2Id[0] = id;
            moveDown(0);
        }
        return res;
    }


    int* binary_dijkstra(int** matrix, int V,int S) {

        int* used = new int[V];
        int* dist = new int[V];
        for (int i = 0; i < V; i++) {
            dist[i] = INT_MAX; used[i] = -1;
        }

        //std::fill(pred, pred + maxnodes, -1);
        //std::fill(prio, prio + maxnodes, INT_MAX);
        dist[S] = 0;
        hsize = 0;
        add(S, 0);

        while (hsize) {
            int u = removeMin();
            for (int e = 0; e < V; e++) {
                int nprio = dist[u] + matrix[e][u];
                if (dist[e] > nprio) {
                    if (dist[e] == INT_MAX) {
                        add(e, nprio);
                    }
                    else {
                        increasePriority(V, nprio);
                    }
                    dist[e] = nprio;
                    used[e] = u;
                }
            }
        }
        return dist;
    }

};

//реализация кучей Фибоначчи

class FibHeap {
public:
    int vertex;
    int dist;

    FibHeap(int vertex, int dist) : vertex(vertex), dist(dist) {}

    // Comparison operator used for sorting in the heap.
    bool operator<(const FibHeap& other) const {
        return dist > other.dist;
    }
};

int* fibonacci_dijkstra(int** matrix,int V,int S) {
    
    int* dist = new int[V];
    bool* used = new bool[V];
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        used[i] = false;
    }
    dist[S] = 0;

    priority_queue<FibHeap> pq;
    pq.push(FibHeap(S, 0));

    while (!pq.empty()) {
        int u = pq.top().vertex;
        pq.pop();

        if (used[u]) {
            continue;
        }
        used[u] = true;

        for (int i = 0; i < V;i++) {
            if (!used[i]) {
                if (dist[i] > dist[u] + matrix[i][u]) {
                    dist[i] = dist[u] + matrix[i][u];
                    pq.push(FibHeap(i, dist[i]));
                }
            }
        }
    }

    return dist;
}

//main
int main()
{
    int el_num;
    //cin >> el_num;
    //el_num = 1000;
    srand((unsigned)time(0));

    ofstream out_std("out_std_2.txt");
    ofstream out_bin("out_bin_2.txt");
    ofstream out_fib("out_fib_2.txt");

    for (int el_num = 10; el_num <= 3000; el_num += 10) {

        int** matrix = standart_fill(el_num, 2);
        //array_out(matrix, el_num);

        //стандартная структура
        auto start = std::chrono::steady_clock::now();
        int* std_dist = standart_dijkstra(matrix, el_num, 0);
        //dist_out(std_dist, el_num);
        auto end = std::chrono::steady_clock::now();

        out_std << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;
        cout << "Std time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;

        start = std::chrono::steady_clock::now();
        int* bin_dist = BinHeap::binary_dijkstra(matrix, el_num, 0);
        //dist_out(bin_dist);
        end = std::chrono::steady_clock::now();

        out_bin << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;
        cout << "Bin time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;

        start = std::chrono::steady_clock::now();
        int* fib_dist = fibonacci_dijkstra(matrix, el_num, 0);

        //dist_out(fib_dist);
        end = std::chrono::steady_clock::now();

        out_fib << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;
        cout << "Fib time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;

    }
}
