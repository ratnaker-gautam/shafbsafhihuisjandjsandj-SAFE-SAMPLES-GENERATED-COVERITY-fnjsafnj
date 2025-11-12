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
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
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
        fprintf(stderr, "Invalid edge\n");
        exit(1);
    }
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

void dfsRecursive(struct Graph* g, int node, int visited[]) {
    if (node < 0 || node >= g->nodes) return;
    if (visited[node]) return;
    
    visited[node] = 1;
    printf("%d ", node);
    
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj[node][i] && !visited[i]) {
            dfsRecursive(g, i, visited);
        }
    }
}

void depthFirstSearch(struct Graph* g, int start) {
    if (start < 0 || start >= g->nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal starting from node %d: ", start);
    dfsRecursive(g, start, visited);
    printf("\n");
}

int isConnectedRecursive(struct Graph* g, int node, int visited[]) {
    if (node < 0 || node >= g->nodes) return 1;
    if (visited[node]) return 1;
    
    visited[node] = 1;
    int connected = 1;
    
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj[node][i] && !visited[i]) {
            connected = isConnectedRecursive(g, i, visited) && connected;
        }
    }
    return connected;
}

int isGraphConnected(struct Graph* g) {
    if (g->nodes == 0) return 1;
    
    int visited[MAX_NODES] = {0};
    int result = isConnectedRecursive(g, 0, visited);
    
    for (int i = 0; i < g->nodes; i++) {
        if (!visited[i]) return 0;
    }
    return result;
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid input for nodes\n");
        return 1;
    }
    
    initGraph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * (nodes - 1) / 2) {
        fprintf(stderr, "Invalid input for edges\n");
        return 1;
    }
    
    printf("Enter edges (u v):\n");
    for (int i = 0; i < edges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        addEdge(&g, u, v);
    }
    
    int start;
    printf("Enter start node for DFS: ");
    if (scanf("%d", &start) != 1 || start < 0 || start >= nodes) {
        fprintf(stderr, "Invalid start node\n");
        return 1;
    }
    
    depthFirstSearch(&g, start);
    
    if (isGraphConnected(&g)) {
        printf("Graph is connected\n");
    } else {
        printf("Graph is not connected\n");
    }
    
    return 0;
}