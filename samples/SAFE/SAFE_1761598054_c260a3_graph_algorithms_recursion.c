//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 20

struct Graph {
    int adj[MAX_NODES][MAX_NODES];
    int numNodes;
};

void initGraph(struct Graph* g, int nodes) {
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

void dfsRecursive(struct Graph* g, int node, int* visited) {
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

int main() {
    struct Graph g;
    int numNodes = 8;
    initGraph(&g, numNodes);
    
    addEdge(&g, 0, 1);
    addEdge(&g, 0, 2);
    addEdge(&g, 1, 3);
    addEdge(&g, 1, 4);
    addEdge(&g, 2, 5);
    addEdge(&g, 2, 6);
    addEdge(&g, 3, 7);
    
    printf("Graph with %d nodes created\n", g.numNodes);
    printf("Edges: 0-1, 0-2, 1-3, 1-4, 2-5, 2-6, 3-7\n");
    
    int startNode;
    printf("Enter starting node for DFS (0-%d): ", g.numNodes - 1);
    if (scanf("%d", &startNode) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    performDFS(&g, startNode);
    
    return 0;
}