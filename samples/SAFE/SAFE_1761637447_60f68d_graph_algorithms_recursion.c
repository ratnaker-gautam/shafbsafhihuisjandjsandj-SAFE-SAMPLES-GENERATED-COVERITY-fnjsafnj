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

void performDFS(struct Graph* g, int start) {
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

int checkConnectivity(struct Graph* g) {
    if (g->nodes == 0) {
        return 1;
    }
    int visited[MAX_NODES] = {0};
    int reached = isConnectedRecursive(g, 0, visited);
    return reached == g->nodes;
}

int readInt(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            continue;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Enter an integer.\n");
            continue;
        }
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d\n", min, max);
    }
}

int main() {
    struct Graph graph;
    int nodes = readInt("Enter number of nodes (1-100): ", 1, MAX_NODES);
    initializeGraph(&graph, nodes);
    
    int edges = readInt("Enter number of edges: ", 0, nodes * (nodes - 1) / 2);
    
    for (int i = 0; i < edges; i++) {
        printf("Edge %d:\n", i + 1);
        int u = readInt("  Enter first node: ", 0, nodes - 1);
        int v = readInt("  Enter second node: ", 0, nodes - 1);
        addEdge(&graph, u, v);
    }
    
    int startNode = readInt("Enter start node for DFS: ", 0, nodes - 1);
    performDFS(&graph, startNode);
    
    if (checkConnectivity(&graph)) {
        printf("Graph is connected\n");
    } else {
        printf("Graph is not connected\n");
    }
    
    return 0;
}