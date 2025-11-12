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
    if (src >= 0 && src < g->nodes && dest >= 0 && dest < g->nodes) {
        g->adj[src][dest] = 1;
        g->adj[dest][src] = 1;
    }
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
    
    printf("DFS traversal starting from node %d: ", start);
    dfsRecursive(g, start, visited);
    printf("\n");
}

int main() {
    struct Graph graph;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (nodes <= 0 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    initGraph(&graph, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (edges < 0 || edges > nodes * (nodes - 1) / 2) {
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
            printf("Invalid node in edge\n");
            return 1;
        }
        addEdge(&graph, src, dest);
    }
    
    int start;
    printf("Enter starting node for DFS: ");
    if (scanf("%d", &start) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    depthFirstSearch(&graph, start);
    
    return 0;
}