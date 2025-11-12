//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

typedef struct {
    int nodes[MAX_NODES][MAX_NODES];
    int size;
} Graph;

void init_graph(Graph *g, int size) {
    if (size <= 0 || size > MAX_NODES) {
        fprintf(stderr, "Invalid graph size\n");
        exit(1);
    }
    g->size = size;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            g->nodes[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void add_edge(Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->size || v < 0 || v >= g->size) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        exit(1);
    }
    g->nodes[u][v] = weight;
}

void floyd_warshall(Graph *g) {
    int dist[MAX_NODES][MAX_NODES];
    for (int i = 0; i < g->size; i++) {
        for (int j = 0; j < g->size; j++) {
            dist[i][j] = g->nodes[i][j];
        }
    }
    
    for (int k = 0; k < g->size; k++) {
        for (int i = 0; i < g->size; i++) {
            for (int j = 0; j < g->size; j++) {
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
    
    for (int i = 0; i < g->size; i++) {
        for (int j = 0; j < g->size; j++) {
            g->nodes[i][j] = dist[i][j];
        }
    }
}

void print_shortest_paths(Graph *g) {
    printf("Shortest path distances:\n");
    for (int i = 0; i < g->size; i++) {
        for (int j = 0; j < g->size; j++) {
            if (g->nodes[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", g->nodes[i][j]);
            }
        }
        printf("\n");
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
    
    if (node_count < 1 || node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        return 1;
    }
    
    init_graph(&g, node_count);
    
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
    
    printf("Enter edges as 'u v weight' (0-indexed):\n");
    for (int i = 0; i < edge_count; i++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&g, u, v, weight);
    }
    
    floyd_warshall(&g);
    print_shortest_paths(&g);
    
    return 0;
}