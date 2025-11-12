//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int nodes;
    int edges;
    int adj[MAX_NODES][MAX_NODES];
};

void initializeGraph(struct Graph* g, int nodes) {
    g->nodes = nodes;
    g->edges = 0;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = 0;
        }
    }
}

int addEdge(struct Graph* g, int u, int v) {
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        return 0;
    }
    if (g->adj[u][v] == 0) {
        g->adj[u][v] = 1;
        g->edges++;
        return 1;
    }
    return 0;
}

void dfsRecursive(struct Graph* g, int node, int visited[]) {
    if (node < 0 || node >= g->nodes || visited[node]) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj[node][i] && !visited[i]) {
            dfsRecursive(g, i, visited);
        }
    }
}

void depthFirstSearch(struct Graph* g, int start) {
    if (start < 0 || start >= g->nodes) {
        printf("Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal starting from node %d: ", start);
    dfsRecursive(g, start, visited);
    printf("\n");
}

int isCyclicUtil(struct Graph* g, int node, int visited[], int recStack[]) {
    if (!visited[node]) {
        visited[node] = 1;
        recStack[node] = 1;
        for (int i = 0; i < g->nodes; i++) {
            if (g->adj[node][i]) {
                if (!visited[i] && isCyclicUtil(g, i, visited, recStack)) {
                    return 1;
                } else if (recStack[i]) {
                    return 1;
                }
            }
        }
    }
    recStack[node] = 0;
    return 0;
}

int isCyclic(struct Graph* g) {
    int visited[MAX_NODES] = {0};
    int recStack[MAX_NODES] = {0};
    for (int i = 0; i < g->nodes; i++) {
        if (!visited[i]) {
            if (isCyclicUtil(g, i, visited, recStack)) {
                return 1;
            }
        }
    }
    return 0;
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    initializeGraph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * nodes) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (u v) as pairs of integers:\n");
    for (int i = 0; i < edges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!addEdge(&g, u, v)) {
            printf("Invalid edge: %d %d\n", u, v);
            return 1;
        }
    }
    
    int start;
    printf("Enter starting node for DFS: ");
    if (scanf("%d", &start) != 1) {
        printf("Invalid start node\n");
        return 1;
    }
    
    depthFirstSearch(&g, start);
    
    if (isCyclic(&g)) {
        printf("Graph contains cycle\n");
    } else {
        printf("Graph does not contain cycle\n");
    }
    
    return 0;
}