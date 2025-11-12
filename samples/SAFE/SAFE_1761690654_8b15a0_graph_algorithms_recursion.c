//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int num_nodes;
    int adj_matrix[MAX_NODES][MAX_NODES];
};

void init_graph(struct Graph *g, int n) {
    if (n <= 0 || n > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->num_nodes = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g->adj_matrix[i][j] = 0;
        }
    }
}

void add_edge(struct Graph *g, int u, int v) {
    if (u < 0 || u >= g->num_nodes || v < 0 || v >= g->num_nodes) {
        fprintf(stderr, "Invalid edge\n");
        exit(1);
    }
    g->adj_matrix[u][v] = 1;
    g->adj_matrix[v][u] = 1;
}

void dfs_recursive(struct Graph *g, int node, int visited[]) {
    if (node < 0 || node >= g->num_nodes) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adj_matrix[node][i] && !visited[i]) {
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

int is_connected_recursive(struct Graph *g, int node, int visited[]) {
    if (node < 0 || node >= g->num_nodes) {
        return 1;
    }
    visited[node] = 1;
    int count = 1;
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adj_matrix[node][i] && !visited[i]) {
            count += is_connected_recursive(g, i, visited);
        }
    }
    return count;
}

int is_connected(struct Graph *g) {
    if (g->num_nodes == 0) {
        return 1;
    }
    int visited[MAX_NODES] = {0};
    int reached = is_connected_recursive(g, 0, visited);
    return reached == g->num_nodes;
}

int main() {
    struct Graph g;
    int num_nodes, num_edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &num_nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    init_graph(&g, num_nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &num_edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Enter edges (u v):\n");
    for (int i = 0; i < num_edges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&g, u, v);
    }
    
    int start_node;
    printf("Enter start node for DFS: ");
    if (scanf("%d", &start_node) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    dfs(&g, start_node);
    
    if (is_connected(&g)) {
        printf("Graph is connected\n");
    } else {
        printf("Graph is not connected\n");
    }
    
    return 0;
}