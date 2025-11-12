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

int hasCycleRecursive(struct Graph* graph, int node, int parent, int visited[]) {
    if (node < 0 || node >= graph->numNodes) {
        return 0;
    }
    visited[node] = 1;
    for (int i = 0; i < graph->numNodes; i++) {
        if (graph->adjMatrix[node][i]) {
            if (!visited[i]) {
                if (hasCycleRecursive(graph, i, node, visited)) {
                    return 1;
                }
            } else if (i != parent) {
                return 1;
            }
        }
    }
    return 0;
}

int detectCycle(struct Graph* graph) {
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < graph->numNodes; i++) {
        if (!visited[i]) {
            if (hasCycleRecursive(graph, i, -1, visited)) {
                return 1;
            }
        }
    }
    return 0;
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
    
    performDFS(&graph, 0);
    
    if (detectCycle(&graph)) {
        printf("Graph contains a cycle\n");
    } else {
        printf("Graph does not contain a cycle\n");
    }
    
    addEdge(&graph, 4, 5);
    
    performDFS(&graph, 0);
    
    if (detectCycle(&graph)) {
        printf("Graph contains a cycle\n");
    } else {
        printf("Graph does not contain a cycle\n");
    }
    
    return 0;
}