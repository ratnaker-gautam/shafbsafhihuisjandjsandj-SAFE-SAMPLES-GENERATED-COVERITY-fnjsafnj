//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 20

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

void depthFirstSearch(struct Graph* graph, int startNode) {
    if (startNode < 0 || startNode >= graph->numNodes) {
        printf("Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal starting from node %d: ", startNode);
    dfsRecursive(graph, startNode, visited);
    printf("\n");
}

int isConnectedRecursive(struct Graph* graph, int node, int visited[]) {
    if (node < 0 || node >= graph->numNodes) {
        return 1;
    }
    visited[node] = 1;
    int count = 1;
    for (int i = 0; i < graph->numNodes; i++) {
        if (graph->adjMatrix[node][i] && !visited[i]) {
            count += isConnectedRecursive(graph, i, visited);
        }
    }
    return count;
}

int isGraphConnected(struct Graph* graph) {
    if (graph->numNodes == 0) {
        return 1;
    }
    int visited[MAX_NODES] = {0};
    int reachableNodes = isConnectedRecursive(graph, 0, visited);
    return reachableNodes == graph->numNodes;
}

int getValidInput(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
    }
}

int main() {
    struct Graph graph;
    int numNodes = getValidInput("Enter number of nodes (1-20): ", 1, MAX_NODES);
    initializeGraph(&graph, numNodes);
    
    int numEdges = getValidInput("Enter number of edges: ", 0, numNodes * (numNodes - 1) / 2);
    
    printf("Adding edges (node indices 0 to %d):\n", numNodes - 1);
    for (int i = 0; i < numEdges; i++) {
        int src = getValidInput("Enter source node: ", 0, numNodes - 1);
        int dest = getValidInput("Enter destination node: ", 0, numNodes - 1);
        addEdge(&graph, src, dest);
    }
    
    int startNode = getValidInput("Enter start node for DFS: ", 0, numNodes - 1);
    depthFirstSearch(&graph, startNode);
    
    if (isGraphConnected(&graph)) {
        printf("The graph is connected.\n");
    } else {
        printf("The graph is not connected.\n");
    }
    
    return 0;
}