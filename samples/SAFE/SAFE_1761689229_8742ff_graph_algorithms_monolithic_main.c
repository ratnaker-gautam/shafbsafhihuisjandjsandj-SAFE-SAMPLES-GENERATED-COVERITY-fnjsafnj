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

void initializeGraph(struct Graph* g, int nodes) {
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void addEdge(struct Graph* g, int u, int v, int weight) {
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) return;
    if (weight < 0) return;
    g->adj[u][v] = weight;
    g->adj[v][u] = weight;
}

void floydWarshall(struct Graph* g, int dist[MAX_NODES][MAX_NODES]) {
    for (int i = 0; i < g->nodes; i++) {
        for (int j = 0; j < g->nodes; j++) {
            dist[i][j] = g->adj[i][j];
        }
    }
    
    for (int k = 0; k < g->nodes; k++) {
        for (int i = 0; i < g->nodes; i++) {
            for (int j = 0; j < g->nodes; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }
}

void printShortestPaths(struct Graph* g, int dist[MAX_NODES][MAX_NODES]) {
    printf("Shortest path distances between all pairs:\n");
    for (int i = 0; i < g->nodes; i++) {
        for (int j = 0; j < g->nodes; j++) {
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
    struct Graph graph;
    int nodes, edges;
    int dist[MAX_NODES][MAX_NODES];
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    initializeGraph(&graph, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * (nodes - 1) / 2) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (u v weight) - nodes are 0-indexed:\n");
    for (int i = 0; i < edges; i++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (u < 0 || u >= nodes || v < 0 || v >= nodes) {
            printf("Invalid node index\n");
            return 1;
        }
        if (weight < 0) {
            printf("Negative weights not allowed\n");
            return 1;
        }
        addEdge(&graph, u, v, weight);
    }
    
    floydWarshall(&graph, dist);
    printShortestPaths(&graph, dist);
    
    printf("\nTesting specific paths:\n");
    for (int i = 0; i < nodes; i++) {
        for (int j = i + 1; j < nodes; j++) {
            if (dist[i][j] == INF) {
                printf("No path between %d and %d\n", i, j);
            } else {
                printf("Shortest distance %d->%d: %d\n", i, j, dist[i][j]);
            }
        }
    }
    
    return 0;
}