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
        nodes = 5;
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
        return;
    }
    if (weight < 0) {
        weight = 1;
    }
    g->adj[u][v] = weight;
    g->adj[v][u] = weight;
}

void floydWarshall(struct Graph* g) {
    int dist[MAX_NODES][MAX_NODES];
    int n = g->nodes;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dist[i][j] = g->adj[i][j];
        }
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            if (dist[i][k] == INF) continue;
            for (int j = 0; j < n; j++) {
                if (dist[k][j] != INF && dist[i][k] != INF) {
                    long long sum = (long long)dist[i][k] + dist[k][j];
                    if (sum > INT_MAX) continue;
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
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

void dfsUtil(struct Graph* g, int v, int visited[]) {
    visited[v] = 1;
    printf("%d ", v);
    
    int n = g->nodes;
    int i = 0;
    while (i < n) {
        if (g->adj[v][i] != INF && g->adj[v][i] != 0 && !visited[i]) {
            dfsUtil(g, i, visited);
        }
        i++;
    }
}

void dfs(struct Graph* g, int start) {
    if (start < 0 || start >= g->nodes) {
        start = 0;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from node %d: ", start);
    dfsUtil(g, start, visited);
    printf("\n");
}

int main() {
    struct Graph g;
    int nodes = 6;
    initializeGraph(&g, nodes);
    
    addEdge(&g, 0, 1, 2);
    addEdge(&g, 0, 2, 4);
    addEdge(&g, 1, 2, 1);
    addEdge(&g, 1, 3, 7);
    addEdge(&g, 2, 4, 3);
    addEdge(&g, 3, 4, 1);
    addEdge(&g, 3, 5, 5);
    addEdge(&g, 4, 5, 2);
    
    printf("Graph with %d nodes\n", g.nodes);
    
    int choice;
    printf("Enter 1 for Floyd-Warshall, 2 for DFS: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    switch (choice) {
        case 1:
            floydWarshall(&g);
            break;
        case 2:
            dfs(&g, 0);
            break;
        default:
            printf("Invalid choice\n");
            break;
    }
    
    return 0;
}