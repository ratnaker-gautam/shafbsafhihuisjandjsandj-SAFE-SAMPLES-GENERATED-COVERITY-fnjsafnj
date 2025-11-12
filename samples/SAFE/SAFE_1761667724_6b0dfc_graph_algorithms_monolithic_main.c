//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct Graph {
    int nodes;
    int adj[MAX_NODES][MAX_NODES];
};

void initGraph(struct Graph* g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void addEdge(struct Graph* g, int u, int v, int weight) {
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        exit(1);
    }
    g->adj[u][v] = weight;
    g->adj[v][u] = weight;
}

void dijkstra(struct Graph* g, int src, int dist[]) {
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INF;
    }
    dist[src] = 0;
    
    for (int count = 0; count < g->nodes - 1; count++) {
        int minDist = INF;
        int minNode = -1;
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && dist[v] <= minDist) {
                minDist = dist[v];
                minNode = v;
            }
        }
        
        if (minNode == -1) break;
        visited[minNode] = 1;
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && g->adj[minNode][v] != INF && dist[minNode] != INF) {
                long newDist = (long)dist[minNode] + g->adj[minNode][v];
                if (newDist < dist[v] && newDist <= INT_MAX) {
                    dist[v] = (int)newDist;
                }
            }
        }
    }
}

void printPath(int parent[], int j) {
    if (parent[j] == -1) {
        printf("%d", j);
        return;
    }
    printPath(parent, parent[j]);
    printf(" -> %d", j);
}

void printSolution(struct Graph* g, int src, int dist[], int parent[]) {
    printf("Node\tDistance\tPath\n");
    for (int i = 0; i < g->nodes; i++) {
        printf("%d\t", i);
        if (dist[i] == INF) {
            printf("INF\t\tNo path\n");
        } else {
            printf("%d\t\t", dist[i]);
            printPath(parent, i);
            printf("\n");
        }
    }
}

int main() {
    struct Graph g;
    int nodes = 6;
    int src = 0;
    
    initGraph(&g, nodes);
    
    addEdge(&g, 0, 1, 4);
    addEdge(&g, 0, 2, 2);
    addEdge(&g, 1, 2, 1);
    addEdge(&g, 1, 3, 5);
    addEdge(&g, 2, 3, 8);
    addEdge(&g, 2, 4, 10);
    addEdge(&g, 3, 4, 2);
    addEdge(&g, 3, 5, 6);
    addEdge(&g, 4, 5, 3);
    
    int dist[MAX_NODES];
    int parent[MAX_NODES];
    
    for (int i = 0; i < nodes; i++) {
        parent[i] = -1;
    }
    
    dijkstra(&g, src, dist);
    
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            if (i != j && g.adj[i][j] != INF && dist[i] != INF) {
                long newDist = (long)dist[i] + g.adj[i][j];
                if (newDist < dist[j] && newDist <= INT_MAX) {
                    dist[j] = (int)newDist;
                    parent[j] = i;
                }
            }
        }
    }
    
    printSolution(&g, src, dist, parent);
    
    return 0;
}