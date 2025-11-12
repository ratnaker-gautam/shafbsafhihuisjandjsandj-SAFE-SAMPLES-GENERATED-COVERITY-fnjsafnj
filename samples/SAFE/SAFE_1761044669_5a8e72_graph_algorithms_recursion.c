//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: graph_algorithms
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

void dfsRecursive(struct Graph* g, int node, int* visited) {
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
    printf("DFS traversal: ");
    dfsRecursive(g, start, visited);
    printf("\n");
}

int findPathRecursive(struct Graph* g, int current, int target, int* visited, int* path, int pathIndex) {
    if (current < 0 || current >= g->nodes || target < 0 || target >= g->nodes) {
        return 0;
    }
    if (current == target) {
        path[pathIndex] = current;
        return 1;
    }
    if (visited[current]) {
        return 0;
    }
    visited[current] = 1;
    path[pathIndex] = current;
    
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj[current][i] && !visited[i]) {
            if (findPathRecursive(g, i, target, visited, path, pathIndex + 1)) {
                return 1;
            }
        }
    }
    visited[current] = 0;
    return 0;
}

void findPath(struct Graph* g, int start, int end) {
    if (start < 0 || start >= g->nodes || end < 0 || end >= g->nodes) {
        printf("Invalid nodes\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    int path[MAX_NODES];
    if (findPathRecursive(g, start, end, visited, path, 0)) {
        printf("Path found: ");
        int i = 0;
        while (1) {
            printf("%d", path[i]);
            if (path[i] == end) break;
            printf(" -> ");
            i++;
        }
        printf("\n");
    } else {
        printf("No path exists between %d and %d\n", start, end);
    }
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
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
    
    printf("Enter edges (u v):\n");
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
    
    int start, end;
    printf("Enter start node for DFS: ");
    if (scanf("%d", &start) != 1) {
        printf("Invalid start node\n");
        return 1;
    }
    depthFirstSearch(&g, start);
    
    printf("Enter start and end nodes for path finding: ");
    if (scanf("%d %d", &start, &end) != 2) {
        printf("Invalid nodes\n");
        return 1;
    }
    findPath(&g, start, end);
    
    return 0;
}