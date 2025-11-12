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
        exit(1);
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

int countConnectedComponents(struct Graph* graph) {
    int visited[MAX_NODES] = {0};
    int count = 0;
    for (int i = 0; i < graph->numNodes; i++) {
        if (!visited[i]) {
            count++;
            dfsRecursive(graph, i, visited);
        }
    }
    return count;
}

int main() {
    struct Graph graph;
    int numNodes, numEdges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &numNodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (numNodes < 1 || numNodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    initializeGraph(&graph, numNodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &numEdges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (numEdges < 0 || numEdges > numNodes * (numNodes - 1) / 2) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (source destination):\n");
    for (int i = 0; i < numEdges; i++) {
        int src, dest;
        if (scanf("%d %d", &src, &dest) != 2) {
            printf("Invalid edge input\n");
            return 1;
        }
        addEdge(&graph, src, dest);
    }
    
    int startNode;
    printf("Enter start node for DFS: ");
    if (scanf("%d", &startNode) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    performDFS(&graph, startNode);
    
    int components = countConnectedComponents(&graph);
    printf("Number of connected components: %d\n", components);
    
    return 0;
}