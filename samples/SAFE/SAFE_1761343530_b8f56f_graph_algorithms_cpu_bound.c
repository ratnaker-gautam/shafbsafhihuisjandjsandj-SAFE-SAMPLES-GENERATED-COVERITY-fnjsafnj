//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
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
                    long long sum = (long long)distances[i][k] + distances[k][j];
                    if (sum < distances[i][j] && sum <= INT_MAX) {
                        distances[i][j] = (int)sum;
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
    
    printf("Enter number of nodes (1-100): ");
    int nodes;
    if (scanf("%d", &nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initialize_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    int edges;
    if (scanf("%d", &edges) != 1 || edges < 0) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'from to weight' (0-indexed):\n");
    for (int i = 0; i < edges; i++) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&g, from, to, weight);
    }
    
    floyd_warshall(&g, distances);
    print_distances(&g, distances);
    
    return 0;
}