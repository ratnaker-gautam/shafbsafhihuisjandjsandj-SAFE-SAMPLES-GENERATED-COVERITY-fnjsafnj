//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
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

void addEdge(struct Graph* g, int src, int dest) {
    if (src < 0 || src >= g->nodes || dest < 0 || dest >= g->nodes) {
        return;
    }
    g->adj[src][dest] = 1;
    g->adj[dest][src] = 1;
}

void dfsRecursive(struct Graph* g, int node, int* visited) {
    if (node < 0 || node >= g->nodes || visited == NULL) {
        return;
    }
    if (visited[node]) {
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
    int count = isConnectedRecursive(g, 0, visited);
    return (count == g->nodes);
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    initGraph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (source destination):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest;
        if (scanf("%d %d", &src, &dest) != 2) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (src < 0 || src >= nodes || dest < 0 || dest >= nodes) {
            printf("Invalid node index\n");
            return 1;
        }
        addEdge(&g, src, dest);
    }
    
    int start;
    printf("Enter starting node for DFS: ");
    if (scanf("%d", &start) != 1 || start < 0 || start >= nodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    
    dfs(&g, start);
    
    if (isConnected(&g)) {
        printf("Graph is connected\n");
    } else {
        printf("Graph is not connected\n");
    }
    
    return 0;
}