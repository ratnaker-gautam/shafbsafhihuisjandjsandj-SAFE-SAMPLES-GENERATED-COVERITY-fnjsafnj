//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int nodes;
    int edges;
    int adj[MAX_NODES][MAX_NODES];
};

void initializeGraph(struct Graph* g, int nodes) {
    g->nodes = nodes;
    g->edges = 0;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = 0;
        }
    }
}

int validateNode(struct Graph* g, int node) {
    return node >= 0 && node < g->nodes;
}

void addEdge(struct Graph* g, int src, int dest) {
    if (!validateNode(g, src) || !validateNode(g, dest)) {
        printf("Invalid node index\n");
        return;
    }
    if (src == dest) {
        printf("Self-loops not allowed\n");
        return;
    }
    if (g->adj[src][dest] == 0) {
        g->adj[src][dest] = 1;
        g->edges++;
    }
}

void dfsRecursive(struct Graph* g, int node, int visited[]) {
    if (!validateNode(g, node) || visited[node]) {
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
    if (!validateNode(g, start)) {
        printf("Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from node %d: ", start);
    dfsRecursive(g, start, visited);
    printf("\n");
}

int findPathRecursive(struct Graph* g, int current, int target, int visited[], int path[], int pathIndex) {
    if (!validateNode(g, current) || !validateNode(g, target)) {
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
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj[current][i] && !visited[i]) {
            if (findPathRecursive(g, i, target, visited, path, pathIndex)) {
                return 1;
            }
        }
    }
    return 0;
}

void findPath(struct Graph* g, int src, int dest) {
    if (!validateNode(g, src) || !validateNode(g, dest)) {
        printf("Invalid node indices\n");
        return;
    }
    if (src == dest) {
        printf("Source and destination are the same\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    int path[MAX_NODES];
    if (!findPathRecursive(g, src, dest, visited, path, 0)) {
        printf("No path exists between %d and %d\n", src, dest);
    }
}

int main() {
    struct Graph g;
    int nodes, edges;
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    initializeGraph(&g, nodes);
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * (nodes - 1)) {
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
        addEdge(&g, src, dest);
    }
    int startNode;
    printf("Enter start node for DFS: ");
    if (scanf("%d", &startNode) != 1) {
        printf("Invalid start node\n");
        return 1;
    }
    depthFirstSearch(&g, startNode);
    int src, dest;
    printf("Enter source and destination for path finding: ");
    if (scanf("%d %d", &src, &dest) != 2) {
        printf("Invalid source/destination\n");
        return 1;
    }
    findPath(&g, src, dest);
    return 0;
}