//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int adjMatrix[MAX_NODES][MAX_NODES];
    int numNodes;
};

void initializeGraph(struct Graph* graph, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    graph->numNodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            graph->adjMatrix[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* graph, int src, int dest) {
    if (src < 0 || src >= graph->numNodes || dest < 0 || dest >= graph->numNodes) {
        fprintf(stderr, "Invalid edge parameters\n");
        exit(1);
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
        if (graph->adjMatrix[node][i] == 1 && !visited[i]) {
            dfsRecursive(graph, i, visited);
        }
    }
}

void performDFS(struct Graph* graph, int startNode) {
    if (graph == NULL || startNode < 0 || startNode >= graph->numNodes) {
        fprintf(stderr, "Invalid DFS parameters\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal starting from node %d: ", startNode);
    dfsRecursive(graph, startNode, visited);
    printf("\n");
}

int isConnected(struct Graph* graph) {
    if (graph == NULL || graph->numNodes == 0) {
        return 0;
    }
    int visited[MAX_NODES] = {0};
    dfsRecursive(graph, 0, visited);
    for (int i = 0; i < graph->numNodes; i++) {
        if (!visited[i]) {
            return 0;
        }
    }
    return 1;
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
    
    printf("Graph with %d nodes created\n", graph.numNodes);
    
    performDFS(&graph, 0);
    
    if (isConnected(&graph)) {
        printf("The graph is connected\n");
    } else {
        printf("The graph is not connected\n");
    }
    
    printf("Testing DFS from different starting points:\n");
    for (int i = 0; i < graph.numNodes; i++) {
        performDFS(&graph, i);
    }
    
    return 0;
}