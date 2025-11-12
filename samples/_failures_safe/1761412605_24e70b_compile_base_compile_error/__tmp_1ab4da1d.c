//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 500

struct Graph {
    int num_nodes;
    int num_edges;
    int *adj_matrix;
};

struct Graph* create_graph(int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) return NULL;
    
    struct Graph *g = malloc(sizeof(struct Graph));
    if (!g) return NULL;
    
    g->num_nodes = nodes;
    g->num_edges = 0;
    
    size_t matrix_size = (size_t)nodes * (size_t)nodes * sizeof(int);
    if (matrix_size > SIZE_MAX / sizeof(int)) {
        free(g);
        return NULL;
    }
    
    g->adj_matrix = calloc((size_t)nodes * (size_t)nodes, sizeof(int));
    if (!g->adj_matrix) {
        free(g);
        return NULL;
    }
    
    return g;
}

void destroy_graph(struct Graph *g) {
    if (g) {
        free(g->adj_matrix);
        free(g);
    }
}

int add_edge(struct Graph *g, int from, int to, int weight) {
    if (!g || from < 0 || to < 0 || from >= g->num_nodes || to >= g->num_nodes) return 0;
    if (weight < 0) return 0;
    
    int *matrix = g->adj_matrix;
    *(matrix + from * g->num_nodes + to) = weight;
    *(matrix + to * g->num_nodes + from) = weight;
    
    g->num_edges++;
    return 1;
}

void dfs_util(struct Graph *g, int node, int *visited) {
    if (!g || !visited || node < 0 || node >= g->num_nodes) return;
    
    printf("%d ", node);
    *(visited + node) = 1;
    
    int *matrix = g->adj_matrix;
    for (int i = 0; i < g->num_nodes; i++) {
        if (*(matrix + node * g->num_nodes + i) > 0 && *(visited + i) == 0) {
            dfs_util(g, i, visited);
        }
    }
}

void depth_first_search(struct Graph *g, int start) {
    if (!g || start < 0 || start >= g->num_nodes) return;
    
    int visited[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        *(visited + i) = 0;
    }
    
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
}

void dijkstra(struct Graph *g, int src) {
    if (!g || src < 0 || src >= g->num_nodes) return;
    
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
        
        int *matrix = g->adj_matrix;
        for (int v = 0; v < g->num_nodes; v++) {
            int weight = *(matrix + min_index * g->num_nodes + v);
            if (weight > 0 && *(visited + v) == 0) {
                int new_dist = *(dist + min_index) + weight;
                if (*(dist + min_index) != INT_MAX && new_dist < *(dist + v)) {
                    *(dist + v) = new_dist;
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", src);
    for (int i = 0; i < g->num_nodes; i++) {
        if (*(dist + i) == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, *(dist + i));
        }
    }
}

int main() {
    struct Graph *g = create_graph(6);
    if (!g) {
        printf("Failed to create graph\n");
        return 1;
    }
    
    add_edge(g, 0, 1, 4);
    add_edge(g, 0, 2, 2);
    add_edge(g, 1, 2, 1);
    add_edge(g, 1, 3, 5);
    add_edge(g, 2, 3, 8);
    add_edge(g, 2, 4, 10);
    add_edge(g, 3, 4, 2);
    add_edge(g, 3, 5, 6);
    add_edge(g, 4, 5, 3);
    
    printf("Graph