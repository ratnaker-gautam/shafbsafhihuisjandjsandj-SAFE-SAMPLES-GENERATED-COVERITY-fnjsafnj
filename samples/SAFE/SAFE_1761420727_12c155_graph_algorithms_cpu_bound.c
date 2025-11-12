//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 100
#define INF INT_MAX

typedef struct {
    int adjacency[MAX_VERTICES][MAX_VERTICES];
    int num_vertices;
} Graph;

void initialize_graph(Graph *g, int vertices) {
    if (vertices <= 0 || vertices > MAX_VERTICES) {
        vertices = 10;
    }
    g->num_vertices = vertices;
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            if (i == j) {
                g->adjacency[i][j] = 0;
            } else {
                g->adjacency[i][j] = INF;
            }
        }
    }
}

void add_edge(Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->num_vertices || v < 0 || v >= g->num_vertices) {
        return;
    }
    if (weight < 0) {
        return;
    }
    g->adjacency[u][v] = weight;
    g->adjacency[v][u] = weight;
}

void floyd_warshall(Graph *g, int dist[MAX_VERTICES][MAX_VERTICES]) {
    for (int i = 0; i < g->num_vertices; i++) {
        for (int j = 0; j < g->num_vertices; j++) {
            dist[i][j] = g->adjacency[i][j];
        }
    }
    
    for (int k = 0; k < g->num_vertices; k++) {
        for (int i = 0; i < g->num_vertices; i++) {
            for (int j = 0; j < g->num_vertices; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }
}

void print_shortest_paths(Graph *g, int dist[MAX_VERTICES][MAX_VERTICES]) {
    printf("Shortest path distances between all pairs:\n");
    for (int i = 0; i < g->num_vertices; i++) {
        for (int j = 0; j < g->num_vertices; j++) {
            if (dist[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", dist[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    Graph g;
    int dist[MAX_VERTICES][MAX_VERTICES];
    
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
    
    floyd_warshall(&g, dist);
    print_shortest_paths(&g, dist);
    
    return 0;
}