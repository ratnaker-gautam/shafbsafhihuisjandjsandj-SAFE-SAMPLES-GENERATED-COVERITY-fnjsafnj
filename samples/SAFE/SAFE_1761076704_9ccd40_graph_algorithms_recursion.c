//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 20

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

int isConnectedRecursive(struct Graph* g, int node, int visited[]) {
    if (node < 0 || node >= g->numNodes) {
        return 1;
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
    if (g->numNodes == 0) {
        return 1;
    }
    int visited[MAX_NODES] = {0};
    int reached = isConnectedRecursive(g, 0, visited);
    return reached == g->numNodes;
}

int main() {
    struct Graph graph;
    initializeGraph(&graph, 6);
    
    addEdge(&graph, 0, 1);
    addEdge(&graph, 0, 2);
    addEdge(&graph, 1, 3);
    addEdge(&graph, 2, 4);
    addEdge(&graph, 3, 5);
    
    printf("Graph with %d nodes\n", graph.numNodes);
    depthFirstSearch(&graph, 0);
    
    if (isGraphConnected(&graph)) {
        printf("Graph is connected\n");
    } else {
        printf("Graph is not connected\n");
    }
    
    struct Graph disconnected;
    initializeGraph(&disconnected, 4);
    addEdge(&disconnected, 0, 1);
    addEdge(&disconnected, 2, 3);
    
    printf("Second graph with %d nodes\n", disconnected.numNodes);
    depthFirstSearch(&disconnected, 0);
    
    if (isGraphConnected(&disconnected)) {
        printf("Graph is connected\n");
    } else {
        printf("Graph is not connected\n");
    }
    
    return 0;
}