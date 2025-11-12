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

void performDFS(struct Graph* g, int start) {
    if (start < 0 || start >= g->numNodes) {
        printf("Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from node %d: ", start);
    dfsRecursive(g, start, visited);
    printf("\n");
}

int findConnectedComponents(struct Graph* g) {
    int visited[MAX_NODES] = {0};
    int count = 0;
    for (int i = 0; i < g->numNodes; i++) {
        if (!visited[i]) {
            printf("Component %d: ", count + 1);
            dfsRecursive(g, i, visited);
            printf("\n");
            count++;
        }
    }
    return count;
}

int main() {
    struct Graph graph;
    int numNodes = 8;
    initializeGraph(&graph, numNodes);
    
    addEdge(&graph, 0, 1);
    addEdge(&graph, 0, 2);
    addEdge(&graph, 1, 3);
    addEdge(&graph, 2, 4);
    addEdge(&graph, 3, 5);
    addEdge(&graph, 4, 5);
    addEdge(&graph, 6, 7);
    
    printf("Graph with %d nodes\n", graph.numNodes);
    printf("Edges: (0-1), (0-2), (1-3), (2-4), (3-5), (4-5), (6-7)\n");
    
    performDFS(&graph, 0);
    performDFS(&graph, 6);
    
    int components = findConnectedComponents(&graph);
    printf("Total connected components: %d\n", components);
    
    return 0;
}