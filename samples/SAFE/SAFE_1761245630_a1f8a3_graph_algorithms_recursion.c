//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int nodes;
    int adjMatrix[MAX_NODES][MAX_NODES];
};

void initializeGraph(struct Graph* g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adjMatrix[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* g, int src, int dest) {
    if (src < 0 || src >= g->nodes || dest < 0 || dest >= g->nodes) {
        fprintf(stderr, "Invalid edge parameters\n");
        exit(1);
    }
    g->adjMatrix[src][dest] = 1;
    g->adjMatrix[dest][src] = 1;
}

void dfsRecursive(struct Graph* g, int node, int visited[]) {
    if (node < 0 || node >= g->nodes) return;
    visited[node] = 1;
    printf("%d ", node);
    
    for (int i = 0; i < g->nodes; i++) {
        if (g->adjMatrix[node][i] && !visited[i]) {
            dfsRecursive(g, i, visited);
        }
    }
}

void depthFirstSearch(struct Graph* g, int start) {
    if (start < 0 || start >= g->nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        visited[i] = 0;
    }
    
    printf("DFS traversal: ");
    dfsRecursive(g, start, visited);
    printf("\n");
}

int isConnectedRecursive(struct Graph* g, int node, int visited[]) {
    if (node < 0 || node >= g->nodes) return 0;
    visited[node] = 1;
    int count = 1;
    
    for (int i = 0; i < g->nodes; i++) {
        if (g->adjMatrix[node][i] && !visited[i]) {
            count += isConnectedRecursive(g, i, visited);
        }
    }
    return count;
}

int isGraphConnected(struct Graph* g) {
    if (g->nodes <= 0) return 0;
    
    int visited[MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        visited[i] = 0;
    }
    
    int reachedNodes = isConnectedRecursive(g, 0, visited);
    return reachedNodes == g->nodes;
}

int findPathRecursive(struct Graph* g, int current, int target, int visited[], int path[], int pathIndex) {
    if (current < 0 || current >= g->nodes || target < 0 || target >= g->nodes) return 0;
    visited[current] = 1;
    path[pathIndex] = current;
    
    if (current == target) {
        return 1;
    }
    
    for (int i = 0; i < g->nodes; i++) {
        if (g->adjMatrix[current][i] && !visited[i]) {
            if (findPathRecursive(g, i, target, visited, path, pathIndex + 1)) {
                return 1;
            }
        }
    }
    
    return 0;
}

void findPath(struct Graph* g, int src, int dest) {
    if (src < 0 || src >= g->nodes || dest < 0 || dest >= g->nodes) {
        fprintf(stderr, "Invalid source or destination\n");
        return;
    }
    
    int visited[MAX_NODES];
    int path[MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        visited[i] = 0;
        path[i] = -1;
    }
    
    if (findPathRecursive(g, src, dest, visited, path, 0)) {
        printf("Path found: ");
        for (int i = 0; i < MAX_NODES && path[i] != -1; i++) {
            printf("%d ", path[i]);
        }
        printf("\n");
    } else {
        printf("No path found between %d and %d\n", src, dest);
    }
}

int main() {
    struct Graph g;
    initializeGraph(&g, 6);
    
    addEdge(&g, 0, 1);
    addEdge(&g, 0, 2);
    addEdge(&g, 1, 3);
    addEdge(&g, 2, 4);
    addEdge(&g, 3, 5);
    addEdge(&g, 4, 5);
    
    depthFirstSearch(&g, 0);
    
    if (isGraphConnected(&g)) {
        printf("Graph is connected\n");
    } else {
        printf("Graph is not connected\n");
    }
    
    findPath(&g, 0, 5);
    findPath(&g, 1, 4);
    
    return 0;
}