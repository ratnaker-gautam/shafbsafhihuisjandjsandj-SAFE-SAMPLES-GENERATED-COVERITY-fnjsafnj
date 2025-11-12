//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct Graph {
    int nodes;
    int adj[MAX_NODES][MAX_NODES];
};

void initializeGraph(struct Graph* g, int nodes) {
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
        return;
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        return;
    }
    g->adj[u][v] = weight;
    g->adj[v][u] = weight;
}

void dijkstra(struct Graph* g, int src) {
    if (src < 0 || src >= g->nodes) {
        fprintf(stderr, "Invalid source node\n");
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[src] = 0;
    
    for (int count = 0; count < g->nodes - 1; count++) {
        int minDist = INF;
        int minIndex = -1;
        
        int i = 0;
        while (i < g->nodes) {
            if (!visited[i] && dist[i] <= minDist) {
                minDist = dist[i];
                minIndex = i;
            }
            i++;
        }
        
        if (minIndex == -1) break;
        
        visited[minIndex] = 1;
        
        int v = 0;
        do {
            if (!visited[v] && g->adj[minIndex][v] != INF && dist[minIndex] != INF) {
                long newDist = (long)dist[minIndex] + g->adj[minIndex][v];
                if (newDist < dist[v] && newDist <= INT_MAX) {
                    dist[v] = (int)newDist;
                }
            }
            v++;
        } while (v < g->nodes);
    }
    
    printf("Shortest distances from node %d:\n", src);
    for (int i = 0; i < g->nodes; i++) {
        if (dist[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph g;
    initializeGraph(&g, 6);
    
    addEdge(&g, 0, 1, 4);
    addEdge(&g, 0, 2, 2);
    addEdge(&g, 1, 2, 1);
    addEdge(&g, 1, 3, 5);
    addEdge(&g, 2, 3, 8);
    addEdge(&g, 2, 4, 10);
    addEdge(&g, 3, 4, 2);
    addEdge(&g, 3, 5, 6);
    addEdge(&g, 4, 5, 3);
    
    printf("Graph with 6 nodes created\n");
    printf("Running Dijkstra's algorithm from node 0:\n");
    dijkstra(&g, 0);
    
    return 0;
}