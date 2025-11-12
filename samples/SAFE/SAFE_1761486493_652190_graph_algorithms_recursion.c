//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 20

struct Graph {
    int adj[MAX_NODES][MAX_NODES];
    int num_nodes;
};

void init_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->num_nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = 0;
        }
    }
}

void add_edge(struct Graph *g, int u, int v) {
    if (u < 0 || u >= g->num_nodes || v < 0 || v >= g->num_nodes) {
        fprintf(stderr, "Invalid edge\n");
        exit(1);
    }
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

void dfs_recursive(struct Graph *g, int node, int visited[]) {
    if (node < 0 || node >= g->num_nodes) {
        return;
    }
    if (visited[node]) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adj[node][i] && !visited[i]) {
            dfs_recursive(g, i, visited);
        }
    }
}

void dfs(struct Graph *g, int start) {
    if (start < 0 || start >= g->num_nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_recursive(g, start, visited);
    printf("\n");
}

int main() {
    struct Graph g;
    int num_nodes = 6;
    init_graph(&g, num_nodes);
    
    add_edge(&g, 0, 1);
    add_edge(&g, 0, 2);
    add_edge(&g, 1, 3);
    add_edge(&g, 1, 4);
    add_edge(&g, 2, 4);
    add_edge(&g, 3, 5);
    add_edge(&g, 4, 5);
    
    printf("Graph with %d nodes\n", g.num_nodes);
    printf("Edges: (0,1) (0,2) (1,3) (1,4) (2,4) (3,5) (4,5)\n");
    
    int start_node;
    printf("Enter starting node for DFS (0-%d): ", num_nodes - 1);
    if (scanf("%d", &start_node) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (start_node < 0 || start_node >= num_nodes) {
        fprintf(stderr, "Start node out of range\n");
        return 1;
    }
    
    dfs(&g, start_node);
    
    return 0;
}