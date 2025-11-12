//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct Graph {
    int nodes;
    int adj[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph *g, int nodes) {
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES) {
        return;
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
}

int add_edge(struct Graph *g, int u, int v, int weight) {
    if (g == NULL || u < 0 || u >= g->nodes || v < 0 || v >= g->nodes || weight < 0) {
        return 0;
    }
    g->adj[u][v] = weight;
    g->adj[v][u] = weight;
    return 1;
}

void floyd_warshall(struct Graph *g, int dist[MAX_NODES][MAX_NODES]) {
    if (g == NULL || dist == NULL) {
        return;
    }
    for (int i = 0; i < g->nodes; i++) {
        for (int j = 0; j < g->nodes; j++) {
            dist[i][j] = g->adj[i][j];
        }
    }
    for (int k = 0; k < g->nodes; k++) {
        for (int i = 0; i < g->nodes; i++) {
            for (int j = 0; j < g->nodes; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }
}

void print_shortest_paths(struct Graph *g, int dist[MAX_NODES][MAX_NODES]) {
    if (g == NULL || dist == NULL) {
        return;
    }
    printf("Shortest path distances:\n");
    for (int i = 0; i < g->nodes; i++) {
        for (int j = 0; j < g->nodes; j++) {
            if (dist[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", dist[i][j]);
            }
        }
        printf("\n");
    }
}

int get_valid_int(int min, int max) {
    int value;
    while (1) {
        if (scanf("%d", &value) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Enter a number between %d and %d: ", min, max);
            continue;
        }
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value out of range. Enter a number between %d and %d: ", min, max);
    }
}

int main() {
    struct Graph graph;
    int nodes, edges;
    int dist[MAX_NODES][MAX_NODES];
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    nodes = get_valid_int(1, MAX_NODES);
    
    initialize_graph(&graph, nodes);
    
    printf("Enter number of edges: ");
    edges = get_valid_int(0, nodes * (nodes - 1) / 2);
    
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, weight;
        printf("Edge %d: ", i + 1);
        u = get_valid_int(0, nodes - 1);
        v = get_valid_int(0, nodes - 1);
        weight = get_valid_int(0, 1000);
        
        if (!add_edge(&graph, u, v, weight)) {
            printf("Failed to add edge. Please try again.\n");
            i--;
        }
    }
    
    floyd_warshall(&graph, dist);
    print_shortest_paths(&graph, dist);
    
    return 0;
}