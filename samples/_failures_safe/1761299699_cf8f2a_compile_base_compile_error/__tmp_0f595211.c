//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int adj[MAX_NODES][MAX_NODES];
    int num_nodes;
};

void init_graph(struct Graph *g, int n) {
    if (n <= 0 || n > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->num_nodes = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
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

int find_path_recursive(struct Graph *g, int current, int target, int visited[], int path[], int *path_index) {
    if (current < 0 || current >= g->num_nodes || target < 0 || target >= g->num_nodes) {
        return 0;
    }
    if (visited[current]) {
        return 0;
    }
    visited[current] = 1;
    path[*path_index] = current;
    (*path_index)++;
    if (current == target) {
        return 1;
    }
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adj[current][i] && !visited[i]) {
            if (find_path_recursive(g, i, target, visited, path, path_index)) {
                return 1;
            }
        }
    }
    (*path_index)--;
    visited[current] = 0;
    return 0;
}

void find_path(struct Graph *g, int start, int end) {
    if (start < 0 || start >= g->num_nodes || end < 0 || end >= g->num_nodes) {
        fprintf(stderr, "Invalid nodes\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    int path[MAX_NODES];
    int path_index = 0;
    if (find_path_recursive(g, start, end, visited, path, &path_index)) {
        printf("Path found: ");
        for (int i = 0; i < path_index; i++) {
            printf("%d ", path[i]);
        }
        printf("\n");
    } else {
        printf("No path exists between %d and %d\n", start, end);
    }
}

int main() {
    struct Graph g;
    int num_nodes, num_edges;
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &num_nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    if (num_nodes < 1 || num_nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    init_graph(&g, num_nodes);
    printf("Enter number of edges: ");
    if (scanf("%d", &num_edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    if (num_edges < 0 || num_edges > num_nodes * (num_nodes - 1) / 2) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    printf("Enter edges (u v):\n");
    for (int i = 0; i < num_edges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (u < 0 || u >= num_nodes || v < 0 || v >= num_nodes) {
            fprintf(stderr, "Invalid edge\n");
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
    int path_start, path_end;
    printf("Enter start and end nodes