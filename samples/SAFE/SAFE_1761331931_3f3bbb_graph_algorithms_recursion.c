//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 20

struct Graph {
    int adjMatrix[MAX_NODES][MAX_NODES];
    int nodeCount;
};

void initializeGraph(struct Graph* graph, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        nodes = MAX_NODES;
    }
    graph->nodeCount = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            graph->adjMatrix[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* graph, int src, int dest) {
    if (src < 0 || src >= graph->nodeCount || dest < 0 || dest >= graph->nodeCount) {
        return;
    }
    graph->adjMatrix[src][dest] = 1;
    graph->adjMatrix[dest][src] = 1;
}

void dfsRecursive(struct Graph* graph, int node, int visited[]) {
    if (node < 0 || node >= graph->nodeCount || visited[node]) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < graph->nodeCount; i++) {
        if (graph->adjMatrix[node][i] && !visited[i]) {
            dfsRecursive(graph, i, visited);
        }
    }
}

void performDFS(struct Graph* graph, int startNode) {
    if (startNode < 0 || startNode >= graph->nodeCount) {
        printf("Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal starting from node %d: ", startNode);
    dfsRecursive(graph, startNode, visited);
    printf("\n");
}

int isConnectedRecursive(struct Graph* graph, int node, int visited[]) {
    if (node < 0 || node >= graph->nodeCount) {
        return 1;
    }
    visited[node] = 1;
    int count = 1;
    for (int i = 0; i < graph->nodeCount; i++) {
        if (graph->adjMatrix[node][i] && !visited[i]) {
            count += isConnectedRecursive(graph, i, visited);
        }
    }
    return count;
}

int checkConnectivity(struct Graph* graph) {
    if (graph->nodeCount == 0) {
        return 1;
    }
    int visited[MAX_NODES] = {0};
    int reachedNodes = isConnectedRecursive(graph, 0, visited);
    return reachedNodes == graph->nodeCount;
}

int main() {
    struct Graph graph;
    int nodeCount = 6;
    initializeGraph(&graph, nodeCount);
    
    addEdge(&graph, 0, 1);
    addEdge(&graph, 0, 2);
    addEdge(&graph, 1, 3);
    addEdge(&graph, 2, 4);
    addEdge(&graph, 3, 5);
    addEdge(&graph, 4, 5);
    
    printf("Graph with %d nodes created\n", graph.nodeCount);
    
    performDFS(&graph, 0);
    performDFS(&graph, 3);
    
    if (checkConnectivity(&graph)) {
        printf("The graph is connected\n");
    } else {
        printf("The graph is not connected\n");
    }
    
    struct Graph disconnectedGraph;
    initializeGraph(&disconnectedGraph, 4);
    addEdge(&disconnectedGraph, 0, 1);
    addEdge(&disconnectedGraph, 2, 3);
    
    if (checkConnectivity(&disconnectedGraph)) {
        printf("The second graph is connected\n");
    } else {
        printf("The second graph is not connected\n");
    }
    
    return 0;
}