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
    if (src < 0 || src >= g->numNodes || dest < 0 || dest >= g->numNodes) {
        return;
    }
    g->adj[src][dest] = 1;
    g->adj[dest][src] = 1;
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

int hasPathRecursive(struct Graph* g, int src, int dest, int visited[]) {
    if (src < 0 || src >= g->numNodes || dest < 0 || dest >= g->numNodes) {
        return 0;
    }
    if (src == dest) {
        return 1;
    }
    visited[src] = 1;
    for (int i = 0; i < g->numNodes; i++) {
        if (g->adj[src][i] && !visited[i]) {
            if (hasPathRecursive(g, i, dest, visited)) {
                return 1;
            }
        }
    }
    return 0;
}

int checkPath(struct Graph* g, int src, int dest) {
    if (src < 0 || src >= g->numNodes || dest < 0 || dest >= g->numNodes) {
        return 0;
    }
    int visited[MAX_NODES] = {0};
    return hasPathRecursive(g, src, dest, visited);
}

int main() {
    struct Graph g;
    initGraph(&g, 6);
    
    addEdge(&g, 0, 1);
    addEdge(&g, 0, 2);
    addEdge(&g, 1, 3);
    addEdge(&g, 2, 4);
    addEdge(&g, 3, 5);
    addEdge(&g, 4, 5);
    
    printf("Graph with 6 nodes created\n");
    printf("Edges: 0-1, 0-2, 1-3, 2-4, 3-5, 4-5\n\n");
    
    depthFirstSearch(&g, 0);
    
    printf("\nPath checks:\n");
    printf("Path from 0 to 5: %s\n", checkPath(&g, 0, 5) ? "Exists" : "Does not exist");
    printf("Path from 1 to 4: %s\n", checkPath(&g, 1, 4) ? "Exists" : "Does not exist");
    printf("Path from 0 to 3: %s\n", checkPath(&g, 0, 3) ? "Exists" : "Does not exist");
    
    return 0;
}