//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 20

struct Graph {
    int adjMatrix[MAX_NODES][MAX_NODES];
    int nodeCount;
};

void initializeGraph(struct Graph* graph, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    
    graph->nodeCount = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            graph->adjMatrix[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* graph, int src, int dest) {
    if (src < 0 || src >= graph->nodeCount || dest < 0 || dest >= graph->nodeCount) {
        fprintf(stderr, "Invalid edge parameters\n");
        exit(1);
    }
    
    graph->adjMatrix[src][dest] = 1;
    graph->adjMatrix[dest][src] = 1;
}

void dfsRecursive(struct Graph* graph, int node, int visited[]) {
    if (node < 0 || node >= graph->nodeCount) return;
    
    visited[node] = 1;
    printf("%d ", node);
    
    for (int i = 0; i < graph->nodeCount; i++) {
        if (graph->adjMatrix[node][i] && !visited[i]) {
            dfsRecursive(graph, i, visited);
        }
    }
}

void depthFirstSearch(struct Graph* graph, int startNode) {
    if (startNode < 0 || startNode >= graph->nodeCount) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal starting from node %d: ", startNode);
    dfsRecursive(graph, startNode, visited);
    printf("\n");
}

int findPathRecursive(struct Graph* graph, int current, int target, int visited[], int path[], int pathIndex) {
    if (current < 0 || current >= graph->nodeCount || target < 0 || target >= graph->nodeCount) {
        return 0;
    }
    
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
    
    for (int i = 0; i < graph->nodeCount; i++) {
        if (graph->adjMatrix[current][i] && !visited[i]) {
            if (findPathRecursive(graph, i, target, visited, path, pathIndex + 1)) {
                return 1;
            }
        }
    }
    
    return 0;
}

void findPath(struct Graph* graph, int start, int end) {
    if (start < 0 || start >= graph->nodeCount || end < 0 || end >= graph->nodeCount) {
        fprintf(stderr, "Invalid path parameters\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int path[MAX_NODES];
    
    if (!findPathRecursive(graph, start, end, visited, path, 0)) {
        printf("No path exists between nodes %d and %d\n", start, end);
    }
}

int main() {
    struct Graph graph;
    initializeGraph(&graph, 6);
    
    addEdge(&graph, 0, 1);
    addEdge(&graph, 0, 2);
    addEdge(&graph, 1, 3);
    addEdge(&graph, 2, 3);
    addEdge(&graph, 3, 4);
    addEdge(&graph, 4, 5);
    
    printf("Graph with %d nodes created\n", graph.nodeCount);
    
    depthFirstSearch(&graph, 0);
    depthFirstSearch(&graph, 3);
    
    findPath(&graph, 0, 5);
    findPath(&graph, 1, 4);
    findPath(&graph, 0, 6);
    
    return 0;
}