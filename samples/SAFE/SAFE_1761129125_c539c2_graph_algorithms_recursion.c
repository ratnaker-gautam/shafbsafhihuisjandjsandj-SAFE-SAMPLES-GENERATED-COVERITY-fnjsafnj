//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
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

void dfsRecursive(struct Graph* graph, int node, int visited[]) {
    if (node < 0 || node >= graph->numNodes || visited[node]) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < graph->numNodes; i++) {
        if (graph->adjMatrix[node][i] && !visited[i]) {
            dfsRecursive(graph, i, visited);
        }
    }
}

void performDFS(struct Graph* graph, int startNode) {
    if (startNode < 0 || startNode >= graph->numNodes) {
        printf("Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal starting from node %d: ", startNode);
    dfsRecursive(graph, startNode, visited);
    printf("\n");
}

int findConnectedComponents(struct Graph* graph) {
    int visited[MAX_NODES] = {0};
    int components = 0;
    for (int i = 0; i < graph->numNodes; i++) {
        if (!visited[i]) {
            components++;
            printf("Component %d: ", components);
            dfsRecursive(graph, i, visited);
            printf("\n");
        }
    }
    return components;
}

int main() {
    struct Graph graph;
    int numNodes = 8;
    initializeGraph(&graph, numNodes);
    
    addEdge(&graph, 0, 1);
    addEdge(&graph, 0, 2);
    addEdge(&graph, 1, 3);
    addEdge(&graph, 2, 4);
    addEdge(&graph, 3, 5);
    addEdge(&graph, 4, 5);
    addEdge(&graph, 6, 7);
    
    printf("Graph Analysis:\n");
    performDFS(&graph, 0);
    performDFS(&graph, 6);
    
    printf("\nConnected Components:\n");
    int totalComponents = findConnectedComponents(&graph);
    printf("Total connected components: %d\n", totalComponents);
    
    return 0;
}