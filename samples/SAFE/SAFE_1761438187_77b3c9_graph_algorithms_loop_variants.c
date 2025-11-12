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

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            if (dist[i][k] != INF) {
                for (int j = 0; j < n; j++) {
                    if (dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
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
    
    int i = 0;
    while (i < g->nodes) {
        if (g->adj[v][i] != INF && g->adj[v][i] != 0 && !visited[i]) {
            dfsUtil(g, i, visited);
        }
        i++;
    }
}

void dfs(struct Graph* g, int start) {
    if (start < 0 || start >= g->nodes) return;
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from %d: ", start);
    dfsUtil(g, start, visited);
    printf("\n");
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
    
    printf("\nGraph algorithms:\n");
    printf("1. Floyd-Warshall (all pairs shortest path)\n");
    printf("2. DFS traversal\n");
    
    int choice;
    printf("Enter choice (1 or 2): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    switch (choice) {
        case 1:
            floydWarshall(&g);
            break;
        case 2: {
            int start;
            printf("Enter start node: ");
            if (scanf("%d", &start) != 1 || start < 0 || start >= nodes) {
                printf("Invalid start node\n");
                return 1;
            }
            dfs(&g, start);
            break;
        }
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    return 0;
}