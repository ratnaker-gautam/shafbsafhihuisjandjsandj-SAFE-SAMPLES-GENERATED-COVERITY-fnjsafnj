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

int isGraphConnected(struct Graph* g) {
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
    int nodes, edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    if (nodes < 1 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    
    initializeGraph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    if (edges < 0 || edges > nodes * (nodes - 1) / 2) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (source destination):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest;
        if (scanf("%d %d", &src, &dest) != 2) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (src < 0 || src >= nodes || dest < 0 || dest >= nodes) {
            fprintf(stderr, "Invalid node in edge\n");
            return 1;
        }
        addEdge(&g, src, dest);
    }
    
    int startNode;
    printf("Enter starting node for DFS: ");
    if (scanf("%d", &startNode) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    if (startNode < 0 || startNode >= nodes) {
        fprintf(stderr, "Invalid starting node\n");
        return 1;
    }
    
    depthFirstSearch(&g, startNode);
    
    if (isGraphConnected(&g)) {
        printf("Graph is connected\n");
    } else {
        printf("Graph is not connected\n");
    }
    
    return 0;
}