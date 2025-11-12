//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int num_nodes;
    int adj_matrix[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph *g, int n) {
    if (g == NULL || n <= 0 || n > MAX_NODES) return;
    g->num_nodes = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            *(*(g->adj_matrix + i) + j) = 0;
        }
    }
}

int add_edge(struct Graph *g, int u, int v, int weight) {
    if (g == NULL || u < 0 || v < 0 || u >= g->num_nodes || v >= g->num_nodes) return 0;
    if (weight < 0) return 0;
    *(*(g->adj_matrix + u) + v) = weight;
    *(*(g->adj_matrix + v) + u) = weight;
    return 1;
}

void dfs_util(struct Graph *g, int node, int *visited) {
    if (g == NULL || visited == NULL || node < 0 || node >= g->num_nodes) return;
    *(visited + node) = 1;
    printf("%d ", node);
    for (int i = 0; i < g->num_nodes; i++) {
        int weight = *(*(g->adj_matrix + node) + i);
        if (weight > 0 && *(visited + i) == 0) {
            dfs_util(g, i, visited);
        }
    }
}

void dfs(struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->num_nodes) return;
    int visited[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        *(visited + i) = 0;
    }
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
}

void dijkstra(struct Graph *g, int src) {
    if (g == NULL || src < 0 || src >= g->num_nodes) return;
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        *(dist + i) = INT_MAX;
        *(visited + i) = 0;
    }
    *(dist + src) = 0;
    for (int count = 0; count < g->num_nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_index = -1;
        for (int v = 0; v < g->num_nodes; v++) {
            if (*(visited + v) == 0 && *(dist + v) <= min_dist) {
                min_dist = *(dist + v);
                min_index = v;
            }
        }
        if (min_index == -1) break;
        *(visited + min_index) = 1;
        for (int v = 0; v < g->num_nodes; v++) {
            int weight = *(*(g->adj_matrix + min_index) + v);
            if (*(visited + v) == 0 && weight > 0 && *(dist + min_index) != INT_MAX) {
                int new_dist = *(dist + min_index) + weight;
                if (new_dist > 0 && new_dist < *(dist + v)) {
                    *(dist + v) = new_dist;
                }
            }
        }
    }
    printf("Dijkstra distances from node %d:\n", src);
    for (int i = 0; i < g->num_nodes; i++) {
        if (*(dist + i) == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, *(dist + i));
        }
    }
}

int main() {
    struct Graph g;
    int num_nodes = 6;
    initialize_graph(&g, num_nodes);
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 2);
    add_edge(&g, 1, 2, 1);
    add_edge(&g, 1, 3, 5);
    add_edge(&g, 2, 3, 8);
    add_edge(&g, 2, 4, 10);
    add_edge(&g, 3, 4, 2);
    add_edge(&g, 3, 5, 6);
    add_edge(&g, 4, 5, 3);
    dfs(&g, 0);
    dijkstra(&g, 0);
    return 0;
}