//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int adj[MAX_NODES][MAX_NODES];
    int numNodes;
};

void initializeGraph(struct Graph* g, int nodes) {
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

void addEdge(struct Graph* g, int u, int v) {
    if (u < 0 || u >= g->numNodes || v < 0 || v >= g->numNodes) {
        fprintf(stderr, "Invalid edge\n");
        exit(1);
    }
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

void dfsRecursive(struct Graph* g, int node, int visited[]) {
    if (node < 0 || node >= g->numNodes) return;
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->numNodes; i++) {
        if (g->adj[node][i] && !visited[i]) {
            dfsRecursive(g, i, visited);
        }
    }
}

void performDFS(struct Graph* g, int start) {
    if (start < 0 || start >= g->numNodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfsRecursive(g, start, visited);
    printf("\n");
}

int findPathRecursive(struct Graph* g, int current, int target, int visited[], int path[], int pathIndex) {
    if (current < 0 || current >= g->numNodes || target < 0 || target >= g->numNodes) return 0;
    if (pathIndex >= MAX_NODES) return 0;
    
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
    
    for (int i = 0; i < g->numNodes; i++) {
        if (g->adj[current][i] && !visited[i]) {
            if (findPathRecursive(g, i, target, visited, path, pathIndex)) {
                return 1;
            }
        }
    }
    
    return 0;
}

void findPath(struct Graph* g, int start, int target) {
    if (start < 0 || start >= g->numNodes || target < 0 || target >= g->numNodes) {
        fprintf(stderr, "Invalid nodes for path finding\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    int path[MAX_NODES];
    if (!findPathRecursive(g, start, target, visited, path, 0)) {
        printf("No path found between %d and %d\n", start, target);
    }
}

int main() {
    struct Graph g;
    int numNodes, numEdges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &numNodes) != 1 || numNodes < 1 || numNodes > MAX_NODES) {
        fprintf(stderr, "Invalid input for nodes\n");
        return 1;
    }
    
    initializeGraph(&g, numNodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &numEdges) != 1 || numEdges < 0 || numEdges > numNodes * (numNodes - 1) / 2) {
        fprintf(stderr, "Invalid input for edges\n");
        return 1;
    }
    
    printf("Enter edges (u v):\n");
    for (int i = 0; i < numEdges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        addEdge(&g, u, v);
    }
    
    int startNode;
    printf("Enter start node for DFS: ");
    if (scanf("%d", &startNode) != 1) {
        fprintf(stderr, "Invalid start node\n");
        return 1;
    }
    performDFS(&g, startNode);
    
    int source, target;
    printf("Enter source and target for path finding: ");
    if (scanf("%d %d", &source, &target) != 2) {
        fprintf(stderr, "Invalid source/target\n");
        return 1;
    }
    findPath(&g, source, target);
    
    return 0;
}