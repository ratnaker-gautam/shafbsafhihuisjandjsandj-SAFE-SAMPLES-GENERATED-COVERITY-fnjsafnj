//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int adj[MAX_NODES][MAX_NODES];
    int numNodes;
};

void initializeGraph(struct Graph* g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->numNodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* g, int u, int v) {
    if (u < 0 || u >= g->numNodes || v < 0 || v >= g->numNodes) {
        fprintf(stderr, "Invalid edge\n");
        exit(1);
    }
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

void dfsRecursive(struct Graph* g, int node, int* visited) {
    if (node < 0 || node >= g->numNodes || visited == NULL) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->numNodes; i++) {
        if (g->adj[node][i] && !visited[i]) {
            dfsRecursive(g, i, visited);
        }
    }
}

void depthFirstSearch(struct Graph* g, int start) {
    if (g == NULL || start < 0 || start >= g->numNodes) {
        fprintf(stderr, "Invalid DFS parameters\n");
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < g->numNodes; i++) {
        visited[i] = 0;
    }
    printf("DFS traversal: ");
    dfsRecursive(g, start, visited);
    printf("\n");
}

int hasCycleRecursive(struct Graph* g, int node, int parent, int* visited) {
    if (node < 0 || node >= g->numNodes || visited == NULL) {
        return 0;
    }
    visited[node] = 1;
    for (int i = 0; i < g->numNodes; i++) {
        if (g->adj[node][i]) {
            if (!visited[i]) {
                if (hasCycleRecursive(g, i, node, visited)) {
                    return 1;
                }
            } else if (i != parent) {
                return 1;
            }
        }
    }
    return 0;
}

int detectCycle(struct Graph* g) {
    if (g == NULL) {
        return 0;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < g->numNodes; i++) {
        visited[i] = 0;
    }
    for (int i = 0; i < g->numNodes; i++) {
        if (!visited[i]) {
            if (hasCycleRecursive(g, i, -1, visited)) {
                return 1;
            }
        }
    }
    return 0;
}

int main() {
    struct Graph g;
    int numNodes = 6;
    initializeGraph(&g, numNodes);
    
    addEdge(&g, 0, 1);
    addEdge(&g, 0, 2);
    addEdge(&g, 1, 3);
    addEdge(&g, 2, 4);
    addEdge(&g, 3, 5);
    
    depthFirstSearch(&g, 0);
    
    if (detectCycle(&g)) {
        printf("Graph contains cycle\n");
    } else {
        printf("Graph does not contain cycle\n");
    }
    
    addEdge(&g, 4, 5);
    
    if (detectCycle(&g)) {
        printf("Graph contains cycle\n");
    } else {
        printf("Graph does not contain cycle\n");
    }
    
    return 0;
}