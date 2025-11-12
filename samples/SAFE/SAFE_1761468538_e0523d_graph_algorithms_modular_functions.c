//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct Graph {
    int nodes;
    int adj[MAX_NODES][MAX_NODES];
};

struct Graph* create_graph(int n) {
    if (n <= 0 || n > MAX_NODES) {
        return NULL;
    }
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->nodes = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            graph->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
    return graph;
}

int add_edge(struct Graph* graph, int u, int v, int weight) {
    if (graph == NULL || u < 0 || u >= graph->nodes || v < 0 || v >= graph->nodes) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    graph->adj[u][v] = weight;
    graph->adj[v][u] = weight;
    return 1;
}

void floyd_warshall(struct Graph* graph, int dist[MAX_NODES][MAX_NODES]) {
    if (graph == NULL || dist == NULL) {
        return;
    }
    for (int i = 0; i < graph->nodes; i++) {
        for (int j = 0; j < graph->nodes; j++) {
            dist[i][j] = graph->adj[i][j];
        }
    }
    for (int k = 0; k < graph->nodes; k++) {
        for (int i = 0; i < graph->nodes; i++) {
            for (int j = 0; j < graph->nodes; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }
}

void print_shortest_paths(struct Graph* graph, int dist[MAX_NODES][MAX_NODES]) {
    if (graph == NULL || dist == NULL) {
        return;
    }
    printf("Shortest path distances between all pairs:\n");
    for (int i = 0; i < graph->nodes; i++) {
        for (int j = 0; j < graph->nodes; j++) {
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
    struct Graph* graph = create_graph(5);
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    
    add_edge(graph, 0, 1, 5);
    add_edge(graph, 0, 3, 10);
    add_edge(graph, 1, 2, 3);
    add_edge(graph, 1, 3, 8);
    add_edge(graph, 2, 4, 2);
    add_edge(graph, 3, 4, 1);
    
    int dist[MAX_NODES][MAX_NODES];
    floyd_warshall(graph, dist);
    print_shortest_paths(graph, dist);
    
    free(graph);
    return 0;
}