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

void depth_first_search(struct Graph *g, int start) {
    if (start < 0 || start >= g->num_nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_recursive(g, start, visited);
    printf("\n");
}

int find_path_recursive(struct Graph *g, int current, int target, int visited[], int path[], int path_index) {
    if (current < 0 || current >= g->num_nodes || target < 0 || target >= g->num_nodes) {
        return 0;
    }
    visited[current] = 1;
    path[path_index] = current;
    path_index++;
    if (current == target) {
        printf("Path found: ");
        for (int i = 0; i < path_index; i++) {
            printf("%d ", path[i]);
        }
        printf("\n");
        return 1;
    }
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adj_matrix[current][i] && !visited[i]) {
            if (find_path_recursive(g, i, target, visited, path, path_index)) {
                return 1;
            }
        }
    }
    return 0;
}

void find_path(struct Graph *g, int start, int target) {
    if (start < 0 || start >= g->num_nodes || target < 0 || target >= g->num_nodes) {
        fprintf(stderr, "Invalid nodes for path finding\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    int path[MAX_NODES];
    if (!find_path_recursive(g, start, target, visited, path, 0)) {
        printf("No path exists between %d and %d\n", start, target);
    }
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
    add_edge(&g, 4, 5);
    depth_first_search(&g, 0);
    find_path(&g, 0, 5);
    find_path(&g, 1, 4);
    return 0;
}