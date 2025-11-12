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

int findPathRecursive(struct Graph* graph, int current, int target, int visited[], int path[], int pathIndex) {
    if (current < 0 || current >= graph->numNodes || target < 0 || target >= graph->numNodes) {
        return 0;
    }
    visited[current] = 1;
    path[pathIndex] = current;
    pathIndex++;
    if (current == target) {
        printf("Path found: ");
        for (int i = 0; i < pathIndex; i++) {
            printf("%d ", path[i]);
        }
        printf("\n");
        return 1;
    }
    for (int i = 0; i < graph->numNodes; i++) {
        if (graph->adjMatrix[current][i] && !visited[i]) {
            if (findPathRecursive(graph, i, target, visited, path, pathIndex)) {
                return 1;
            }
        }
    }
    return 0;
}

void findPath(struct Graph* graph, int start, int end) {
    if (start < 0 || start >= graph->numNodes || end < 0 || end >= graph->numNodes) {
        printf("Invalid nodes\n");
        return;
    }
    if (start == end) {
        printf("Start and end are the same node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    int path[MAX_NODES];
    printf("Finding path from %d to %d: ", start, end);
    if (!findPathRecursive(graph, start, end, visited, path, 0)) {
        printf("No path exists\n");
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
    addEdge(&graph, 3, 6);
    addEdge(&graph, 4, 7);
    addEdge(&graph, 5, 7);
    depthFirstSearch(&graph, 0);
    findPath(&graph, 0, 7);
    findPath(&graph, 0, 6);
    findPath(&graph, 2, 4);
    return 0;
}