//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 20

struct Graph {
    int adj[MAX_NODES][MAX_NODES];
    int numNodes;
};

void initGraph(struct Graph* g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->numNodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* g, int src, int dest) {
    if (src < 0 || src >= g->numNodes || dest < 0 || dest >= g->numNodes) {
        fprintf(stderr, "Invalid edge\n");
        exit(1);
    }
    g->adj[src][dest] = 1;
    g->adj[dest][src] = 1;
}

void dfsRecursive(struct Graph* g, int node, int visited[]) {
    if (node < 0 || node >= g->numNodes) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->numNodes; i++) {
        if (g->adj[node][i] && !visited[i]) {
            dfsRecursive(g, i, visited);
        }
    }
}

void depthFirstSearch(struct Graph* g, int start) {
    if (start < 0 || start >= g->numNodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfsRecursive(g, start, visited);
    printf("\n");
}

int hasCycleUtil(struct Graph* g, int node, int visited[], int parent) {
    if (node < 0 || node >= g->numNodes) {
        return 0;
    }
    visited[node] = 1;
    for (int i = 0; i < g->numNodes; i++) {
        if (g->adj[node][i]) {
            if (!visited[i]) {
                if (hasCycleUtil(g, i, visited, node)) {
                    return 1;
                }
            } else if (i != parent) {
                return 1;
            }
        }
    }
    return 0;
}

int hasCycle(struct Graph* g) {
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < g->numNodes; i++) {
        if (!visited[i]) {
            if (hasCycleUtil(g, i, visited, -1)) {
                return 1;
            }
        }
    }
    return 0;
}

int main() {
    struct Graph g;
    int numNodes = 5;
    initGraph(&g, numNodes);
    
    addEdge(&g, 0, 1);
    addEdge(&g, 0, 2);
    addEdge(&g, 1, 3);
    addEdge(&g, 2, 4);
    
    printf("Graph with %d nodes\n", g.numNodes);
    depthFirstSearch(&g, 0);
    
    if (hasCycle(&g)) {
        printf("Graph contains cycle\n");
    } else {
        printf("Graph does not contain cycle\n");
    }
    
    struct Graph g2;
    initGraph(&g2, 3);
    addEdge(&g2, 0, 1);
    addEdge(&g2, 1, 2);
    addEdge(&g2, 2, 0);
    
    if (hasCycle(&g2)) {
        printf("Second graph contains cycle\n");
    } else {
        printf("Second graph does not contain cycle\n");
    }
    
    return 0;
}