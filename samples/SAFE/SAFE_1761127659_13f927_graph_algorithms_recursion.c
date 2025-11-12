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

void performDFS(struct Graph* g, int start) {
    if (start < 0 || start >= g->numNodes) {
        printf("Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from node %d: ", start);
    dfsRecursive(g, start, visited);
    printf("\n");
}

int isConnected(struct Graph* g, int node1, int node2, int visited[]) {
    if (node1 < 0 || node1 >= g->numNodes || node2 < 0 || node2 >= g->numNodes) {
        return 0;
    }
    if (node1 == node2) {
        return 1;
    }
    visited[node1] = 1;
    for (int i = 0; i < g->numNodes; i++) {
        if (g->adj[node1][i] && !visited[i]) {
            if (isConnected(g, i, node2, visited)) {
                return 1;
            }
        }
    }
    return 0;
}

void checkConnection(struct Graph* g, int node1, int node2) {
    if (node1 < 0 || node1 >= g->numNodes || node2 < 0 || node2 >= g->numNodes) {
        printf("Invalid nodes\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    if (isConnected(g, node1, node2, visited)) {
        printf("Nodes %d and %d are connected\n", node1, node2);
    } else {
        printf("Nodes %d and %d are not connected\n", node1, node2);
    }
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
    addEdge(&g, 4, 5);
    
    printf("Graph with %d nodes created\n", g.numNodes);
    printf("Edges: 0-1, 0-2, 1-3, 2-4, 3-5, 4-5\n");
    
    performDFS(&g, 0);
    performDFS(&g, 3);
    
    checkConnection(&g, 0, 5);
    checkConnection(&g, 1, 4);
    checkConnection(&g, 0, 6);
    
    return 0;
}