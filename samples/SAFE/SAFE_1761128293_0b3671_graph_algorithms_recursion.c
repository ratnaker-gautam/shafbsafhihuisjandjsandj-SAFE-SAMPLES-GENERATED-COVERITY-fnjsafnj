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
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    graph->adjMatrix[src][dest] = 1;
    graph->adjMatrix[dest][src] = 1;
}

void dfsRecursive(struct Graph* graph, int node, int* visited) {
    if (node < 0 || node >= graph->numNodes) {
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

void depthFirstSearch(struct Graph* graph, int startNode) {
    if (startNode < 0 || startNode >= graph->numNodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal starting from node %d: ", startNode);
    dfsRecursive(graph, startNode, visited);
    printf("\n");
}

int main() {
    struct Graph graph;
    int numNodes, numEdges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &numNodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initializeGraph(&graph, numNodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &numEdges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (numEdges < 0 || numEdges > numNodes * (numNodes - 1) / 2) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (source destination):\n");
    for (int i = 0; i < numEdges; i++) {
        int src, dest;
        if (scanf("%d %d", &src, &dest) != 2) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        addEdge(&graph, src, dest);
    }
    
    int startNode;
    printf("Enter starting node for DFS: ");
    if (scanf("%d", &startNode) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    depthFirstSearch(&graph, startNode);
    
    return 0;
}