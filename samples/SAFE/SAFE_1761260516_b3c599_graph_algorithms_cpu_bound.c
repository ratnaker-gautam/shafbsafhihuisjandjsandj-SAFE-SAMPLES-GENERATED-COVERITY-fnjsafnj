//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int matrix[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

void initialize_graph(Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->node_count = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->matrix[i][j] = (i == j) ? 0 : INT_MAX;
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
    g->matrix[from][to] = weight;
}

void floyd_warshall(Graph *g) {
    int dist[MAX_NODES][MAX_NODES];
    for (int i = 0; i < g->node_count; i++) {
        for (int j = 0; j < g->node_count; j++) {
            dist[i][j] = g->matrix[i][j];
        }
    }

    for (int k = 0; k < g->node_count; k++) {
        for (int i = 0; i < g->node_count; i++) {
            for (int j = 0; j < g->node_count; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                    long long sum = (long long)dist[i][k] + dist[k][j];
                    if (sum < dist[i][j] && sum <= INT_MAX) {
                        dist[i][j] = (int)sum;
                    }
                }
            }
        }
    }

    for (int i = 0; i < g->node_count; i++) {
        for (int j = 0; j < g->node_count; j++) {
            g->matrix[i][j] = dist[i][j];
        }
    }
}

void print_shortest_paths(Graph *g) {
    printf("Shortest path distances:\n");
    for (int i = 0; i < g->node_count; i++) {
        for (int j = 0; j < g->node_count; j++) {
            if (g->matrix[i][j] == INT_MAX) {
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
    int nodes = 5;
    initialize_graph(&g, nodes);

    add_edge(&g, 0, 1, 5);
    add_edge(&g, 0, 3, 10);
    add_edge(&g, 1, 2, 3);
    add_edge(&g, 1, 3, 2);
    add_edge(&g, 2, 4, 1);
    add_edge(&g, 3, 2, 2);
    add_edge(&g, 3, 4, 4);
    add_edge(&g, 4, 0, 7);

    floyd_warshall(&g);
    print_shortest_paths(&g);

    return 0;
}