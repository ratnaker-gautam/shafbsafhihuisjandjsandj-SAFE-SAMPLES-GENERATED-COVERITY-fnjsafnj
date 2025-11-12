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

int hasPathRecursive(struct Graph* g, int current, int target, int visited[]) {
    if (current < 0 || current >= g->numNodes || target < 0 || target >= g->numNodes) {
        return 0;
    }
    if (current == target) {
        return 1;
    }
    visited[current] = 1;
    for (int i = 0; i < g->numNodes; i++) {
        if (g->adj[current][i] && !visited[i]) {
            if (hasPathRecursive(g, i, target, visited)) {
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
    int numNodes = 6;
    initializeGraph(&g, numNodes);
    
    addEdge(&g, 0, 1);
    addEdge(&g, 0, 2);
    addEdge(&g, 1, 3);
    addEdge(&g, 2, 4);
    addEdge(&g, 3, 5);
    
    printf("Graph with %d nodes\n", g.numNodes);
    depthFirstSearch(&g, 0);
    
    int path1 = checkPath(&g, 0, 5);
    printf("Path from 0 to 5: %s\n", path1 ? "Yes" : "No");
    
    int path2 = checkPath(&g, 1, 4);
    printf("Path from 1 to 4: %s\n", path2 ? "Yes" : "No");
    
    int path3 = checkPath(&g, 5, 0);
    printf("Path from 5 to 0: %s\n", path3 ? "Yes" : "No");
    
    return 0;
}