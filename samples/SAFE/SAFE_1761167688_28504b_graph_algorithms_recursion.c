//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int nodes;
    int edges;
    int adj[MAX_NODES][MAX_NODES];
};

void initGraph(struct Graph* g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        nodes = MAX_NODES;
    }
    g->nodes = nodes;
    g->edges = 0;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = 0;
        }
    }
}

int addEdge(struct Graph* g, int u, int v) {
    if (g == NULL || u < 0 || u >= g->nodes || v < 0 || v >= g->nodes || u == v) {
        return 0;
    }
    if (g->adj[u][v] == 0) {
        g->adj[u][v] = 1;
        g->adj[v][u] = 1;
        g->edges++;
        return 1;
    }
    return 0;
}

void dfsRecursive(struct Graph* g, int node, int* visited) {
    if (g == NULL || visited == NULL || node < 0 || node >= g->nodes) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj[node][i] == 1 && visited[i] == 0) {
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
    printf("DFS traversal starting from node %d: ", start);
    dfsRecursive(g, start, visited);
    printf("\n");
}

int isConnectedRecursive(struct Graph* g, int node, int* visited) {
    if (g == NULL || visited == NULL || node < 0 || node >= g->nodes) {
        return 0;
    }
    visited[node] = 1;
    int count = 1;
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj[node][i] == 1 && visited[i] == 0) {
            count += isConnectedRecursive(g, i, visited);
        }
    }
    return count;
}

int isConnected(struct Graph* g) {
    if (g == NULL || g->nodes == 0) {
        return 1;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        visited[i] = 0;
    }
    int count = isConnectedRecursive(g, 0, visited);
    return count == g->nodes;
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes. Using default: 5\n");
        nodes = 5;
    }
    
    initGraph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        printf("Invalid number of edges. Using default: 0\n");
        edges = 0;
    }
    
    printf("Enter edges (u v) for %d edges:\n", edges);
    for (int i = 0; i < edges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            printf("Invalid edge input. Skipping remaining edges.\n");
            break;
        }
        if (!addEdge(&g, u, v)) {
            printf("Invalid edge (%d, %d). Skipping.\n", u, v);
        }
    }
    
    int start;
    printf("Enter starting node for DFS: ");
    if (scanf("%d", &start) != 1 || start < 0 || start >= g.nodes) {
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