//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int nodes;
    int adj[MAX_NODES][MAX_NODES];
};

void initGraph(struct Graph* g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        nodes = MAX_NODES;
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* g, int u, int v) {
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        return;
    }
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

void dfsRecursive(struct Graph* g, int node, int* visited) {
    if (node < 0 || node >= g->nodes || visited == NULL) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj[node][i] && !visited[i]) {
            dfsRecursive(g, i, visited);
        }
    }
}

void dfs(struct Graph* g, int start) {
    if (g == NULL || start < 0 || start >= g->nodes) {
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        visited[i] = 0;
    }
    printf("DFS traversal: ");
    dfsRecursive(g, start, visited);
    printf("\n");
}

int isConnectedRecursive(struct Graph* g, int node, int* visited) {
    if (node < 0 || node >= g->nodes || visited == NULL) {
        return 0;
    }
    visited[node] = 1;
    int count = 1;
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj[node][i] && !visited[i]) {
            count += isConnectedRecursive(g, i, visited);
        }
    }
    return count;
}

int isConnected(struct Graph* g) {
    if (g == NULL || g->nodes == 0) {
        return 0;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        visited[i] = 0;
    }
    int reached = isConnectedRecursive(g, 0, visited);
    return reached == g->nodes;
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes. Using default 5.\n");
        nodes = 5;
    }
    
    initGraph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        printf("Invalid number of edges. Using default 4.\n");
        edges = 4;
    }
    
    printf("Enter edges (u v) for %d edges:\n", edges);
    for (int i = 0; i < edges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            printf("Invalid edge input. Skipping remaining edges.\n");
            break;
        }
        if (u < 0 || u >= nodes || v < 0 || v >= nodes) {
            printf("Invalid node indices. Edge skipped.\n");
            continue;
        }
        addEdge(&g, u, v);
    }
    
    int start;
    printf("Enter starting node for DFS: ");
    if (scanf("%d", &start) != 1 || start < 0 || start >= nodes) {
        printf("Invalid start node. Using 0.\n");
        start = 0;
    }
    
    dfs(&g, start);
    
    if (isConnected(&g)) {
        printf("Graph is connected.\n");
    } else {
        printf("Graph is not connected.\n");
    }
    
    return 0;
}