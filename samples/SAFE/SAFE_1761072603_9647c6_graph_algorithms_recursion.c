//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int numNodes;
    int adjMatrix[MAX_NODES][MAX_NODES];
};

void initializeGraph(struct Graph* graph, int numNodes) {
    if (numNodes <= 0 || numNodes > MAX_NODES) {
        numNodes = MAX_NODES;
    }
    graph->numNodes = numNodes;
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            graph->adjMatrix[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* graph, int src, int dest) {
    if (src < 0 || src >= graph->numNodes || dest < 0 || dest >= graph->numNodes) {
        return;
    }
    graph->adjMatrix[src][dest] = 1;
    graph->adjMatrix[dest][src] = 1;
}

void dfsRecursive(struct Graph* graph, int node, int* visited) {
    if (node < 0 || node >= graph->numNodes || visited == NULL) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < graph->numNodes; i++) {
        if (graph->adjMatrix[node][i] == 1 && visited[i] == 0) {
            dfsRecursive(graph, i, visited);
        }
    }
}

void performDFS(struct Graph* graph, int startNode) {
    if (graph == NULL || startNode < 0 || startNode >= graph->numNodes) {
        printf("Invalid input for DFS\n");
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < graph->numNodes; i++) {
        visited[i] = 0;
    }
    printf("DFS traversal starting from node %d: ", startNode);
    dfsRecursive(graph, startNode, visited);
    printf("\n");
}

int isConnected(struct Graph* graph) {
    if (graph == NULL || graph->numNodes <= 0) {
        return 0;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < graph->numNodes; i++) {
        visited[i] = 0;
    }
    int count = 0;
    void dfsCount(int node) {
        if (node < 0 || node >= graph->numNodes) {
            return;
        }
        visited[node] = 1;
        count++;
        for (int i = 0; i < graph->numNodes; i++) {
            if (graph->adjMatrix[node][i] == 1 && visited[i] == 0) {
                dfsCount(i);
            }
        }
    }
    dfsCount(0);
    return count == graph->numNodes;
}

int main() {
    struct Graph graph;
    int numNodes = 6;
    initializeGraph(&graph, numNodes);
    addEdge(&graph, 0, 1);
    addEdge(&graph, 0, 2);
    addEdge(&graph, 1, 3);
    addEdge(&graph, 2, 4);
    addEdge(&graph, 3, 5);
    addEdge(&graph, 4, 5);
    performDFS(&graph, 0);
    if (isConnected(&graph)) {
        printf("The graph is connected.\n");
    } else {
        printf("The graph is not connected.\n");
    }
    printf("Testing with invalid inputs:\n");
    performDFS(&graph, -1);
    performDFS(&graph, 10);
    struct Graph emptyGraph;
    initializeGraph(&emptyGraph, 0);
    performDFS(&emptyGraph, 0);
    return 0;
}