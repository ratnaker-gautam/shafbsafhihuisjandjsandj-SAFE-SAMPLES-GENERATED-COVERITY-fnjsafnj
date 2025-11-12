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

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->size || to < 0 || to >= g->size) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        exit(1);
    }
    g->nodes[from][to] = weight;
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
    int node_count = 5;
    
    init_graph(&g, node_count);
    
    add_edge(&g, 0, 1, 5);
    add_edge(&g, 0, 3, 10);
    add_edge(&g, 1, 2, 3);
    add_edge(&g, 1, 3, 2);
    add_edge(&g, 2, 4, 1);
    add_edge(&g, 3, 2, 2);
    add_edge(&g, 3, 4, 6);
    add_edge(&g, 4, 0, 7);
    
    printf("Graph with %d nodes created\n", node_count);
    printf("Running Floyd-Warshall algorithm...\n");
    
    floyd_warshall(&g);
    
    print_shortest_paths(&g);
    
    return 0;
}