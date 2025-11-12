//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 20

struct Graph {
    int adjMatrix[MAX_NODES][MAX_NODES];
    int numNodes;
};

void initializeGraph(struct Graph* graph, int nodes) {
    if (graph == NULL || nodes <= 0 || nodes > MAX_NODES) return;
    
    graph->numNodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            graph->adjMatrix[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* graph, int src, int dest) {
    if (graph == NULL) return;
    if (src < 0 || src >= graph->numNodes) return;
    if (dest < 0 || dest >= graph->numNodes) return;
    
    graph->adjMatrix[src][dest] = 1;
    graph->adjMatrix[dest][src] = 1;
}

void dfsRecursive(struct Graph* graph, int vertex, int visited[]) {
    if (graph == NULL) return;
    if (vertex < 0 || vertex >= graph->numNodes) return;
    if (visited == NULL) return;
    
    visited[vertex] = 1;
    printf("%d ", vertex);
    
    for (int i = 0; i < graph->numNodes; i++) {
        if (graph->adjMatrix[vertex][i] == 1 && !visited[i]) {
            dfsRecursive(graph, i, visited);
        }
    }
}

void performDFS(struct Graph* graph, int startVertex) {
    if (graph == NULL) return;
    if (startVertex < 0 || startVertex >= graph->numNodes) return;
    
    int visited[MAX_NODES];
    for (int i = 0; i < graph->numNodes; i++) {
        visited[i] = 0;
    }
    
    printf("DFS traversal starting from vertex %d: ", startVertex);
    dfsRecursive(graph, startVertex, visited);
    printf("\n");
}

int findPathRecursive(struct Graph* graph, int current, int target, int visited[], int path[], int pathIndex) {
    if (graph == NULL) return 0;
    if (current < 0 || current >= graph->numNodes) return 0;
    if (target < 0 || target >= graph->numNodes) return 0;
    if (visited == NULL) return 0;
    if (path == NULL) return 0;
    if (pathIndex < 0 || pathIndex >= graph->numNodes) return 0;
    
    visited[current] = 1;
    path[pathIndex] = current;
    
    if (current == target) {
        printf("Path found: ");
        for (int i = 0; i <= pathIndex; i++) {
            printf("%d ", path[i]);
        }
        printf("\n");
        return 1;
    }
    
    for (int i = 0; i < graph->numNodes; i++) {
        if (graph->adjMatrix[current][i] == 1 && !visited[i]) {
            if (findPathRecursive(graph, i, target, visited, path, pathIndex + 1)) {
                return 1;
            }
        }
    }
    
    return 0;
}

void findPath(struct Graph* graph, int start, int end) {
    if (graph == NULL) return;
    if (start < 0 || start >= graph->numNodes) return;
    if (end < 0 || end >= graph->numNodes) return;
    if (start == end) {
        printf("Start and end are the same vertex: %d\n", start);
        return;
    }
    
    int visited[MAX_NODES];
    int path[MAX_NODES];
    for (int i = 0; i < graph->numNodes; i++) {
        visited[i] = 0;
        path[i] = -1;
    }
    
    printf("Finding path from %d to %d: ", start, end);
    if (!findPathRecursive(graph, start, end, visited, path, 0)) {
        printf("No path found\n");
    }
}

int main() {
    struct Graph graph;
    initializeGraph(&graph, 8);
    
    addEdge(&graph, 0, 1);
    addEdge(&graph, 0, 2);
    addEdge(&graph, 1, 3);
    addEdge(&graph, 1, 4);
    addEdge(&graph, 2, 5);
    addEdge(&graph, 2, 6);
    addEdge(&graph, 3, 7);
    addEdge(&graph, 4, 7);
    addEdge(&graph, 5, 7);
    addEdge(&graph, 6, 7);
    
    performDFS(&graph, 0);
    findPath(&graph, 0, 7);
    findPath(&graph, 0, 3);
    findPath(&graph, 1, 6);
    
    return 0;
}