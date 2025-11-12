//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

void initialize_graph(Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    g->node_count = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adjacency[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        exit(1);
    }
    g->adjacency[from][to] = weight;
}

void floyd_warshall(Graph *g, int distances[MAX_NODES][MAX_NODES]) {
    for (int i = 0; i < g->node_count; i++) {
        for (int j = 0; j < g->node_count; j++) {
            distances[i][j] = g->adjacency[i][j];
        }
    }
    
    for (int k = 0; k < g->node_count; k++) {
        for (int i = 0; i < g->node_count; i++) {
            for (int j = 0; j < g->node_count; j++) {
                if (distances[i][k] != INF && distances[k][j] != INF) {
                    int sum;
                    if (distances[i][k] > INT_MAX - distances[k][j]) {
                        sum = INF;
                    } else {
                        sum = distances[i][k] + distances[k][j];
                    }
                    if (sum < distances[i][j]) {
                        distances[i][j] = sum;
                    }
                }
            }
        }
    }
}

void print_distances(Graph *g, int distances[MAX_NODES][MAX_NODES]) {
    printf("Shortest path distances between all pairs:\n");
    for (int i = 0; i < g->node_count; i++) {
        for (int j = 0; j < g->node_count; j++) {
            if (distances[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", distances[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    Graph g;
    int distances[MAX_NODES][MAX_NODES];
    
    initialize_graph(&g, 6);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 2);
    add_edge(&g, 1, 2, 1);
    add_edge(&g, 1, 3, 5);
    add_edge(&g, 2, 3, 8);
    add_edge(&g, 2, 4, 10);
    add_edge(&g, 3, 4, 2);
    add_edge(&g, 3, 5, 6);
    add_edge(&g, 4, 5, 3);
    
    floyd_warshall(&g, distances);
    print_distances(&g, distances);
    
    return 0;
}