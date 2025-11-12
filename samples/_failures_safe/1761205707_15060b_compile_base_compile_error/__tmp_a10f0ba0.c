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

void addEdge(struct Graph* g, int u, int v) {
    if (!validateNode(g, u) || !validateNode(g, v)) {
        printf("Invalid node(s)\n");
        return;
    }
    if (u == v) {
        printf("Self-loop not allowed\n");
        return;
    }
    if (g->adj[u][v] == 0) {
        g->adj[u][v] = 1;
        g->adj[v][u] = 1;
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

int isConnectedRecursive(struct Graph* g, int node, int visited[]) {
    if (!validateNode(g, node)) {
        return 1;
    }
    visited[node] = 1;
    int count = 1;
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj[node][i] && !visited[i]) {
            count += isConnectedRecursive(g, i, visited);
        }
    }
    return count;
}

int isGraphConnected(struct Graph* g) {
    if (g->nodes == 0) {
        return 1;
    }
    int visited[MAX_NODES] = {0};
    int reachable = isConnectedRecursive(g, 0, visited);
    return reachable == g->nodes;
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

void findPath(struct Graph* g, int start, int end) {
    if (!validateNode(g, start) || !validateNode(g, end)) {
        printf("Invalid node(s)\n");
        return;
    }
    if (start == end) {
        printf("Start and end are the same node\n");
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
    int nodes, edges;
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    initializeGraph(&g, nodes);
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * (nodes - 1) / 2) {
        printf("Invalid number of edges\n");
        return 1;
    }
    printf("Enter edges (u v):\n");
    for (int i = 0; i < edges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            printf("Invalid edge input\n");
            return 1;
        }
        addEdge(&g, u, v);
    }
    printf("\nGraph operations:\n");
    depthFirstSearch(&g, 0);
    if (isGraphConnected(&g)) {
        printf("Graph is connected