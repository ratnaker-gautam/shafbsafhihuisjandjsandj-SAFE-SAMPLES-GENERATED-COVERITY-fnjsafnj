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

void depthFirstSearch(struct Graph* graph, int startNode) {
    if (graph == NULL || startNode < 0 || startNode >= graph->numNodes) {
        fprintf(stderr, "Invalid DFS parameters\n");
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

int findPathRecursive(struct Graph* graph, int current, int target, int* visited, int* path, int pathIndex) {
    if (current < 0 || current >= graph->numNodes || target < 0 || target >= graph->numNodes) {
        return 0;
    }
    
    if (visited == NULL || path == NULL) {
        return 0;
    }
    
    visited[current] = 1;
    path[pathIndex] = current;
    
    if (current == target) {
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
    if (graph == NULL || start < 0 || start >= graph->numNodes || end < 0 || end >= graph->numNodes) {
        fprintf(stderr, "Invalid path parameters\n");
        return;
    }
    
    int visited[MAX_NODES];
    int path[MAX_NODES];
    for (int i = 0; i < graph->numNodes; i++) {
        visited[i] = 0;
        path[i] = -1;
    }
    
    printf("Path from %d to %d: ", start, end);
    if (findPathRecursive(graph, start, end, visited, path, 0)) {
        for (int i = 0; i < MAX_NODES && path[i] != -1; i++) {
            printf("%d ", path[i]);
        }
    } else {
        printf("No path exists");
    }
    printf("\n");
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
    
    depthFirstSearch(&graph, 0);
    findPath(&graph, 0, 5);
    findPath(&graph, 1, 4);
    
    return 0;
}