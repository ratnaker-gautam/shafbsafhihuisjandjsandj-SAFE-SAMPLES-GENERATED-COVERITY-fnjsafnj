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

int findPathRecursive(struct Graph* g, int current, int target, int visited[], int path[], int pathIndex) {
    if (current < 0 || current >= g->numNodes || target < 0 || target >= g->numNodes) {
        return 0;
    }
    visited[current] = 1;
    path[pathIndex] = current;
    pathIndex++;
    
    if (current == target) {
        printf("Path found: ");
        for (int i = 0; i < pathIndex; i++) {
            printf("%d ", path[i]);
        }
        printf("\n");
        return 1;
    }
    
    for (int i = 0; i < g->numNodes; i++) {
        if (g->adj[current][i] && !visited[i]) {
            if (findPathRecursive(g, i, target, visited, path, pathIndex)) {
                return 1;
            }
        }
    }
    return 0;
}

void findPath(struct Graph* g, int start, int end) {
    if (start < 0 || start >= g->numNodes || end < 0 || end >= g->numNodes) {
        printf("Invalid nodes\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    int path[MAX_NODES];
    if (!findPathRecursive(g, start, end, visited, path, 0)) {
        printf("No path found between %d and %d\n", start, end);
    }
}

int main() {
    struct Graph g;
    initGraph(&g, 8);
    
    addEdge(&g, 0, 1);
    addEdge(&g, 0, 2);
    addEdge(&g, 1, 3);
    addEdge(&g, 1, 4);
    addEdge(&g, 2, 5);
    addEdge(&g, 2, 6);
    addEdge(&g, 3, 7);
    addEdge(&g, 4, 7);
    
    printf("Graph with %d nodes\n", g.numNodes);
    depthFirstSearch(&g, 0);
    findPath(&g, 0, 7);
    findPath(&g, 0, 10);
    
    return 0;
}