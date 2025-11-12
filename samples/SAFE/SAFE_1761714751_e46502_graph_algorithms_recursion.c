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

void addEdge(struct Graph* g, int u, int v) {
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        return;
    }
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

void dfsRecursive(struct Graph* g, int node, int visited[]) {
    if (node < 0 || node >= g->nodes || visited[node]) {
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

void depthFirstSearch(struct Graph* g, int start) {
    if (start < 0 || start >= g->nodes) {
        printf("Invalid start node\n");
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

int hasPathRecursive(struct Graph* g, int current, int target, int visited[]) {
    if (current < 0 || current >= g->nodes || target < 0 || target >= g->nodes) {
        return 0;
    }
    if (current == target) {
        return 1;
    }
    visited[current] = 1;
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj[current][i] && !visited[i]) {
            if (hasPathRecursive(g, i, target, visited)) {
                return 1;
            }
        }
    }
    return 0;
}

int checkPath(struct Graph* g, int u, int v) {
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        return 0;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        visited[i] = 0;
    }
    return hasPathRecursive(g, u, v, visited);
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid input\n");
        return 1;
    }
    
    initGraph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter edges (u v):\n");
    for (int i = 0; i < edges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            printf("Invalid input\n");
            return 1;
        }
        if (u < 0 || u >= nodes || v < 0 || v >= nodes) {
            printf("Invalid node\n");
            return 1;
        }
        addEdge(&g, u, v);
    }
    
    int start;
    printf("Enter start node for DFS: ");
    if (scanf("%d", &start) != 1 || start < 0 || start >= nodes) {
        printf("Invalid start node\n");
        return 1;
    }
    
    depthFirstSearch(&g, start);
    
    int u, v;
    printf("Enter two nodes to check path (u v): ");
    if (scanf("%d %d", &u, &v) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    if (u < 0 || u >= nodes || v < 0 || v >= nodes) {
        printf("Invalid nodes\n");
        return 1;
    }
    
    if (checkPath(&g, u, v)) {
        printf("Path exists between %d and %d\n", u, v);
    } else {
        printf("No path between %d and %d\n", u, v);
    }
    
    return 0;
}