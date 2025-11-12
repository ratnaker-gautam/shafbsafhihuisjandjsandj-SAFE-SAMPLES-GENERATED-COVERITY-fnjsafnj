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
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void addEdge(struct Graph* g, int u, int v, int weight) {
    if (u >= 0 && u < g->nodes && v >= 0 && v < g->nodes && weight >= 0) {
        g->adj[u][v] = weight;
        g->adj[v][u] = weight;
    }
}

void floydWarshall(struct Graph* g) {
    int dist[MAX_NODES][MAX_NODES];
    int n = g->nodes;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dist[i][j] = g->adj[i][j];
        }
    }

    int k = 0;
    while (k < n) {
        int i = 0;
        do {
            int j = 0;
            while (j < n) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    long sum = (long)dist[i][k] + dist[k][j];
                    if (sum < dist[i][j] && sum <= INT_MAX) {
                        dist[i][j] = (int)sum;
                    }
                }
                j++;
            }
            i++;
        } while (i < n);
        k++;
    }

    printf("Shortest path distances:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (dist[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", dist[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    initializeGraph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * (nodes - 1) / 2) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (u v weight):\n");
    for (int count = 0; count < edges; count++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (u < 0 || u >= nodes || v < 0 || v >= nodes || w < 0) {
            printf("Invalid edge parameters\n");
            return 1;
        }
        addEdge(&g, u, v, w);
    }
    
    floydWarshall(&g);
    
    return 0;
}