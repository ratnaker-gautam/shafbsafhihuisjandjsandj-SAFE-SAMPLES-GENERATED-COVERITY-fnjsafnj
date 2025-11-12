//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 20

struct Graph {
    int adjMatrix[MAX_NODES][MAX_NODES];
    int numNodes;
};

void initializeGraph(struct Graph* g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        nodes = MAX_NODES;
    }
    g->numNodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adjMatrix[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* g, int src, int dest) {
    if (src < 0 || src >= g->numNodes || dest < 0 || dest >= g->numNodes) {
        return;
    }
    g->adjMatrix[src][dest] = 1;
    g->adjMatrix[dest][src] = 1;
}

void dfsRecursive(struct Graph* g, int node, int visited[]) {
    if (node < 0 || node >= g->numNodes || visited[node]) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->numNodes; i++) {
        if (g->adjMatrix[node][i] && !visited[i]) {
            dfsRecursive(g, i, visited);
        }
    }
}

void depthFirstSearch(struct Graph* g, int start) {
    if (start < 0 || start >= g->numNodes) {
        printf("Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from node %d: ", start);
    dfsRecursive(g, start, visited);
    printf("\n");
}

int hasPathRecursive(struct Graph* g, int current, int target, int visited[]) {
    if (current < 0 || current >= g->numNodes || target < 0 || target >= g->numNodes) {
        return 0;
    }
    if (current == target) {
        return 1;
    }
    visited[current] = 1;
    for (int i = 0; i < g->numNodes; i++) {
        if (g->adjMatrix[current][i] && !visited[i]) {
            if (hasPathRecursive(g, i, target, visited)) {
                return 1;
            }
        }
    }
    return 0;
}

int checkPath(struct Graph* g, int src, int dest) {
    if (src < 0 || src >= g->numNodes || dest < 0 || dest >= g->numNodes) {
        return 0;
    }
    int visited[MAX_NODES] = {0};
    return hasPathRecursive(g, src, dest, visited);
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
    
    printf("Graph with %d nodes created\n", graph.numNodes);
    
    depthFirstSearch(&graph, 0);
    depthFirstSearch(&graph, 3);
    
    printf("Path from 0 to 5: %s\n", checkPath(&graph, 0, 5) ? "Exists" : "Does not exist");
    printf("Path from 1 to 4: %s\n", checkPath(&graph, 1, 4) ? "Exists" : "Does not exist");
    printf("Path from 0 to 6: %s\n", checkPath(&graph, 0, 6) ? "Exists" : "Does not exist");
    
    return 0;
}