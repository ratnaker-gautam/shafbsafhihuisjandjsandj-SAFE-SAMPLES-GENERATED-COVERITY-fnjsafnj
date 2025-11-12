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

void floyd_warshall(Graph *g, int dist[MAX_NODES][MAX_NODES]) {
    for (int i = 0; i < g->node_count; i++) {
        for (int j = 0; j < g->node_count; j++) {
            dist[i][j] = g->adjacency[i][j];
        }
    }
    
    for (int k = 0; k < g->node_count; k++) {
        for (int i = 0; i < g->node_count; i++) {
            for (int j = 0; j < g->node_count; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }
}

int main() {
    Graph g;
    int nodes = 5;
    initialize_graph(&g, nodes);
    
    add_edge(&g, 0, 1, 5);
    add_edge(&g, 0, 3, 10);
    add_edge(&g, 1, 2, 3);
    add_edge(&g, 2, 3, 1);
    add_edge(&g, 3, 4, 2);
    add_edge(&g, 4, 0, 7);
    
    int distances[MAX_NODES][MAX_NODES];
    floyd_warshall(&g, distances);
    
    printf("Shortest path distances between all pairs:\n");
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            if (distances[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", distances[i][j]);
            }
        }
        printf("\n");
    }
    
    printf("\nSample paths:\n");
    printf("Distance from 0 to 2: ");
    if (distances[0][2] == INF) {
        printf("No path\n");
    } else {
        printf("%d\n", distances[0][2]);
    }
    
    printf("Distance from 3 to 0: ");
    if (distances[3][0] == INF) {
        printf("No path\n");
    } else {
        printf("%d\n", distances[3][0]);
    }
    
    printf("Distance from 1 to 4: ");
    if (distances[1][4] == INF) {
        printf("No path\n");
    } else {
        printf("%d\n", distances[1][4]);
    }
    
    return 0;
}