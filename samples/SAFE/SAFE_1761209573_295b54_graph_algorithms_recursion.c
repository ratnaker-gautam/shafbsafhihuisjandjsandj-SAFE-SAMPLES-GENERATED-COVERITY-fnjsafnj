//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int nodes;
    int adj[MAX_NODES][MAX_NODES];
};

void initGraph(struct Graph* g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        nodes = MAX_NODES;
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* g, int u, int v) {
    if (u >= 0 && u < g->nodes && v >= 0 && v < g->nodes) {
        g->adj[u][v] = 1;
        g->adj[v][u] = 1;
    }
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
    if (visited[current]) {
        return 0;
    }
    visited[current] = 1;
    path[pathIndex] = current;
    if (current == target) {
        return 1;
    }
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj[current][i] && !visited[i]) {
            if (findPathRecursive(g, i, target, visited, path, pathIndex + 1)) {
                return 1;
            }
        }
    }
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
        while (i < MAX_NODES && visited[path[i]]) {
            printf("%d ", path[i]);
            if (path[i] == end) break;
            i++;
        }
        printf("\n");
    } else {
        printf("No path exists\n");
    }
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid input\n");
        return 1;
    }
    
    initGraph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter edges (u v):\n");
    for (int i = 0; i < edges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            printf("Invalid input\n");
            return 1;
        }
        if (u < 0 || u >= nodes || v < 0 || v >= nodes) {
            printf("Invalid edge\n");
            return 1;
        }
        addEdge(&g, u, v);
    }
    
    int start, end;
    printf("Enter start node for DFS: ");
    if (scanf("%d", &start) != 1 || start < 0 || start >= nodes) {
        printf("Invalid start node\n");
        return 1;
    }
    depthFirstSearch(&g, start);
    
    printf("Enter start and end nodes for path finding: ");
    if (scanf("%d %d", &start, &end) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    if (start < 0 || start >= nodes || end < 0 || end >= nodes) {
        printf("Invalid nodes\n");
        return 1;
    }
    findPath(&g, start, end);
    
    return 0;
}