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
    if (nodes <= 0 || nodes > MAX_NODES) {
        nodes = MAX_NODES;
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
    printf("DFS traversal: ");
    dfsRecursive(graph, startNode, visited);
    printf("\n");
}

int hasPathRecursive(struct Graph* graph, int current, int target, int visited[]) {
    if (current < 0 || current >= graph->numNodes || target < 0 || target >= graph->numNodes) {
        return 0;
    }
    if (current == target) {
        return 1;
    }
    visited[current] = 1;
    for (int i = 0; i < graph->numNodes; i++) {
        if (graph->adjMatrix[current][i] && !visited[i]) {
            if (hasPathRecursive(graph, i, target, visited)) {
                return 1;
            }
        }
    }
    return 0;
}

int checkPath(struct Graph* graph, int src, int dest) {
    if (src < 0 || src >= graph->numNodes || dest < 0 || dest >= graph->numNodes) {
        return 0;
    }
    int visited[MAX_NODES] = {0};
    return hasPathRecursive(graph, src, dest, visited);
}

int main() {
    struct Graph graph;
    initializeGraph(&graph, 6);
    
    addEdge(&graph, 0, 1);
    addEdge(&graph, 0, 2);
    addEdge(&graph, 1, 3);
    addEdge(&graph, 2, 4);
    addEdge(&graph, 3, 5);
    addEdge(&graph, 4, 5);
    
    printf("Graph operations:\n");
    depthFirstSearch(&graph, 0);
    
    int from = 0, to = 5;
    if (checkPath(&graph, from, to)) {
        printf("Path exists from %d to %d\n", from, to);
    } else {
        printf("No path from %d to %d\n", from, to);
    }
    
    from = 1;
    to = 4;
    if (checkPath(&graph, from, to)) {
        printf("Path exists from %d to %d\n", from, to);
    } else {
        printf("No path from %d to %d\n", from, to);
    }
    
    return 0;
}