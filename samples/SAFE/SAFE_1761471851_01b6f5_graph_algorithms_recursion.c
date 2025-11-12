//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int adj[MAX_NODES][MAX_NODES];
    int num_nodes;
};

void init_graph(struct Graph* g, int n) {
    if (n <= 0 || n > MAX_NODES) {
        exit(1);
    }
    g->num_nodes = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g->adj[i][j] = 0;
        }
    }
}

void add_edge(struct Graph* g, int u, int v) {
    if (u < 0 || u >= g->num_nodes || v < 0 || v >= g->num_nodes) {
        exit(1);
    }
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

void dfs_recursive(struct Graph* g, int node, int* visited) {
    if (node < 0 || node >= g->num_nodes || visited == NULL) {
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

void dfs(struct Graph* g, int start) {
    if (g == NULL || start < 0 || start >= g->num_nodes) {
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        visited[i] = 0;
    }
    printf("DFS traversal: ");
    dfs_recursive(g, start, visited);
    printf("\n");
}

int is_connected_recursive(struct Graph* g, int node, int* visited) {
    if (node < 0 || node >= g->num_nodes || visited == NULL) {
        return 0;
    }
    visited[node] = 1;
    int count = 1;
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adj[node][i] && !visited[i]) {
            count += is_connected_recursive(g, i, visited);
        }
    }
    return count;
}

int is_connected(struct Graph* g) {
    if (g == NULL || g->num_nodes <= 0) {
        return 0;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        visited[i] = 0;
    }
    int reached = is_connected_recursive(g, 0, visited);
    return reached == g->num_nodes;
}

int main() {
    struct Graph g;
    int num_nodes = 6;
    init_graph(&g, num_nodes);
    
    add_edge(&g, 0, 1);
    add_edge(&g, 0, 2);
    add_edge(&g, 1, 3);
    add_edge(&g, 2, 4);
    add_edge(&g, 3, 5);
    
    dfs(&g, 0);
    
    if (is_connected(&g)) {
        printf("Graph is connected\n");
    } else {
        printf("Graph is not connected\n");
    }
    
    struct Graph g2;
    init_graph(&g2, 4);
    add_edge(&g2, 0, 1);
    add_edge(&g2, 2, 3);
    
    if (is_connected(&g2)) {
        printf("Graph is connected\n");
    } else {
        printf("Graph is not connected\n");
    }
    
    return 0;
}