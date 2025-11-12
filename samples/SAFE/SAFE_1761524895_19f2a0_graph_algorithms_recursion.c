//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int adj[MAX_NODES][MAX_NODES];
    int numNodes;
};

void initGraph(struct Graph* g, int nodes) {
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

void addEdge(struct Graph* g, int u, int v) {
    if (u < 0 || u >= g->numNodes || v < 0 || v >= g->numNodes) {
        fprintf(stderr, "Invalid edge\n");
        exit(1);
    }
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
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

void dfs(struct Graph* g, int start) {
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

int isConnectedRecursive(struct Graph* g, int node, int* visited) {
    if (node < 0 || node >= g->numNodes || visited == NULL) {
        return 0;
    }
    visited[node] = 1;
    int count = 1;
    for (int i = 0; i < g->numNodes; i++) {
        if (g->adj[node][i] && !visited[i]) {
            count += isConnectedRecursive(g, i, visited);
        }
    }
    return count;
}

int isConnected(struct Graph* g) {
    if (g == NULL || g->numNodes == 0) {
        return 0;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < g->numNodes; i++) {
        visited[i] = 0;
    }
    int reached = isConnectedRecursive(g, 0, visited);
    return reached == g->numNodes;
}

int main() {
    struct Graph g;
    int numNodes, numEdges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &numNodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initGraph(&g, numNodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &numEdges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (numEdges < 0 || numEdges > numNodes * (numNodes - 1) / 2) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (u v):\n");
    for (int i = 0; i < numEdges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        addEdge(&g, u, v);
    }
    
    int startNode;
    printf("Enter starting node for DFS: ");
    if (scanf("%d", &startNode) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    dfs(&g, startNode);
    
    if (isConnected(&g)) {
        printf("Graph is connected\n");
    } else {
        printf("Graph is not connected\n");
    }
    
    return 0;
}