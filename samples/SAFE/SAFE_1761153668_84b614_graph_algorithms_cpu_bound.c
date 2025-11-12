//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

void initialize_graph(Graph *g, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid edge\n");
        exit(1);
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weight not allowed\n");
        exit(1);
    }
    g->adjacency[from][to] = weight;
}

void floyd_warshall(Graph *g, int distances[MAX_NODES][MAX_NODES]) {
    for (int i = 0; i < g->node_count; i++) {
        for (int j = 0; j < g->node_count; j++) {
            if (i == j) {
                distances[i][j] = 0;
            } else if (g->adjacency[i][j] != 0) {
                distances[i][j] = g->adjacency[i][j];
            } else {
                distances[i][j] = INT_MAX / 2;
            }
        }
    }

    for (int k = 0; k < g->node_count; k++) {
        for (int i = 0; i < g->node_count; i++) {
            for (int j = 0; j < g->node_count; j++) {
                if (distances[i][k] + distances[k][j] < distances[i][j]) {
                    distances[i][j] = distances[i][k] + distances[k][j];
                }
            }
        }
    }
}

int main() {
    Graph g;
    int node_count;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &node_count) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initialize_graph(&g, node_count);
    
    int edge_count;
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (edge_count < 0 || edge_count > node_count * node_count) {
        fprintf(stderr, "Invalid edge count\n");
        return 1;
    }
    
    printf("Enter edges as 'from to weight' (0-indexed):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&g, from, to, weight);
    }
    
    int distances[MAX_NODES][MAX_NODES];
    floyd_warshall(&g, distances);
    
    printf("\nShortest path distances:\n");
    for (int i = 0; i < g.node_count; i++) {
        for (int j = 0; j < g.node_count; j++) {
            if (distances[i][j] == INT_MAX / 2) {
                printf("INF ");
            } else {
                printf("%3d ", distances[i][j]);
            }
        }
        printf("\n");
    }
    
    return 0;
}