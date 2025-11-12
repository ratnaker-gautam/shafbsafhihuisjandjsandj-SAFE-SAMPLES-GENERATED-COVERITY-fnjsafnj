//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

typedef struct {
    int matrix[MAX_NODES][MAX_NODES];
    int num_nodes;
} Graph;

void initialize_graph(Graph *g, int n) {
    if (n <= 0 || n > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->num_nodes = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                g->matrix[i][j] = 0;
            } else {
                g->matrix[i][j] = INF;
            }
        }
    }
}

void add_edge(Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->num_nodes || v < 0 || v >= g->num_nodes) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        exit(1);
    }
    g->matrix[u][v] = weight;
    g->matrix[v][u] = weight;
}

void floyd_warshall(Graph *g) {
    int dist[MAX_NODES][MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        for (int j = 0; j < g->num_nodes; j++) {
            dist[i][j] = g->matrix[i][j];
        }
    }
    
    for (int k = 0; k < g->num_nodes; k++) {
        for (int i = 0; i < g->num_nodes; i++) {
            for (int j = 0; j < g->num_nodes; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        if (dist[i][k] > INF - dist[k][j]) {
                            continue;
                        }
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < g->num_nodes; i++) {
        for (int j = 0; j < g->num_nodes; j++) {
            g->matrix[i][j] = dist[i][j];
        }
    }
}

void print_shortest_paths(Graph *g) {
    printf("Shortest path distances:\n");
    for (int i = 0; i < g->num_nodes; i++) {
        for (int j = 0; j < g->num_nodes; j++) {
            if (g->matrix[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", g->matrix[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    Graph g;
    int num_nodes = 5;
    
    initialize_graph(&g, num_nodes);
    
    add_edge(&g, 0, 1, 5);
    add_edge(&g, 0, 2, 9);
    add_edge(&g, 0, 3, 1);
    add_edge(&g, 1, 2, 2);
    add_edge(&g, 1, 4, 7);
    add_edge(&g, 2, 3, 6);
    add_edge(&g, 3, 4, 4);
    
    floyd_warshall(&g);
    print_shortest_paths(&g);
    
    return 0;
}