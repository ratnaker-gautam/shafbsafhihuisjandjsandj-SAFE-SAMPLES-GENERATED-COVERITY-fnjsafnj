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
    int reachableCount = isConnectedRecursive(graph, 0, visited);
    return reachableCount == graph->numNodes;
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
    
    printf("Graph with %d nodes\n", graph.numNodes);
    depthFirstSearch(&graph, 0);
    
    if (isGraphConnected(&graph)) {
        printf("The graph is connected\n");
    } else {
        printf("The graph is not connected\n");
    }
    
    struct Graph disconnectedGraph;
    initializeGraph(&disconnectedGraph, 4);
    addEdge(&disconnectedGraph, 0, 1);
    addEdge(&disconnectedGraph, 2, 3);
    
    printf("Second graph with 4 nodes\n");
    depthFirstSearch(&disconnectedGraph, 0);
    
    if (isGraphConnected(&disconnectedGraph)) {
        printf("The second graph is connected\n");
    } else {
        printf("The second graph is not connected\n");
    }
    
    return 0;
}