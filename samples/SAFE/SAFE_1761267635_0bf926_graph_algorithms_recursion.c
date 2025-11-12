//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 20

struct Graph {
    int adj[MAX_NODES][MAX_NODES];
    int numNodes;
};

void initializeGraph(struct Graph* g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        nodes = MAX_NODES;
    }
    g->numNodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* g, int src, int dest) {
    if (src >= 0 && src < g->numNodes && dest >= 0 && dest < g->numNodes) {
        g->adj[src][dest] = 1;
        g->adj[dest][src] = 1;
    }
}

void dfsRecursive(struct Graph* g, int node, int visited[]) {
    if (node < 0 || node >= g->numNodes || visited[node]) {
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
    if (start < 0 || start >= g->numNodes) {
        printf("Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfsRecursive(g, start, visited);
    printf("\n");
}

int isConnected(struct Graph* g) {
    if (g->numNodes == 0) return 1;
    int visited[MAX_NODES] = {0};
    dfsRecursive(g, 0, visited);
    for (int i = 0; i < g->numNodes; i++) {
        if (!visited[i]) return 0;
    }
    return 1;
}

int main() {
    struct Graph graph;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid input\n");
        return 1;
    }
    
    initializeGraph(&graph, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter edges (source destination):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest;
        if (scanf("%d %d", &src, &dest) != 2) {
            printf("Invalid input\n");
            return 1;
        }
        if (src < 0 || src >= nodes || dest < 0 || dest >= nodes) {
            printf("Invalid node\n");
            return 1;
        }
        addEdge(&graph, src, dest);
    }
    
    int start;
    printf("Enter start node for DFS: ");
    if (scanf("%d", &start) != 1 || start < 0 || start >= nodes) {
        printf("Invalid start node\n");
        return 1;
    }
    
    depthFirstSearch(&graph, start);
    
    if (isConnected(&graph)) {
        printf("Graph is connected\n");
    } else {
        printf("Graph is not connected\n");
    }
    
    return 0;
}