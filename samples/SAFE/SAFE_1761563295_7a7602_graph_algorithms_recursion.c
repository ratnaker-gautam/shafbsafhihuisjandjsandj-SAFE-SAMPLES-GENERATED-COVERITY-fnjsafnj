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

void addEdge(struct Graph* g, int src, int dest) {
    if (src < 0 || src >= g->numNodes || dest < 0 || dest >= g->numNodes) {
        fprintf(stderr, "Invalid edge\n");
        exit(1);
    }
    g->adj[src][dest] = 1;
    g->adj[dest][src] = 1;
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

int findPathRecursive(struct Graph* g, int current, int target, int* visited, int* path, int pathIndex) {
    if (current < 0 || current >= g->numNodes || target < 0 || target >= g->numNodes) {
        return 0;
    }
    if (visited == NULL || path == NULL) {
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
    if (g == NULL || start < 0 || start >= g->numNodes || end < 0 || end >= g->numNodes) {
        fprintf(stderr, "Invalid path parameters\n");
        return;
    }
    int visited[MAX_NODES];
    int path[MAX_NODES];
    for (int i = 0; i < g->numNodes; i++) {
        visited[i] = 0;
    }
    if (!findPathRecursive(g, start, end, visited, path, 0)) {
        printf("No path exists between %d and %d\n", start, end);
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
    depthFirstSearch(&g, 0);
    findPath(&g, 0, 5);
    findPath(&g, 1, 4);
    return 0;
}