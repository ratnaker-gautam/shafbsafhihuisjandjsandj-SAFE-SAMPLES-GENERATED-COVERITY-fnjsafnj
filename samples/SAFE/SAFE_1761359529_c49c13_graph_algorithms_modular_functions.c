//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct Graph {
    int nodes;
    int matrix[MAX_NODES][MAX_NODES];
};

struct Graph* create_graph(int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        return NULL;
    }
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            graph->matrix[i][j] = (i == j) ? 0 : INF;
        }
    }
    return graph;
}

int add_edge(struct Graph* graph, int from, int to, int weight) {
    if (graph == NULL || from < 0 || from >= graph->nodes || to < 0 || to >= graph->nodes) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    graph->matrix[from][to] = weight;
    return 1;
}

void floyd_warshall(struct Graph* graph) {
    if (graph == NULL) {
        return;
    }
    int dist[MAX_NODES][MAX_NODES];
    for (int i = 0; i < graph->nodes; i++) {
        for (int j = 0; j < graph->nodes; j++) {
            dist[i][j] = graph->matrix[i][j];
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
    for (int i = 0; i < graph->nodes; i++) {
        for (int j = 0; j < graph->nodes; j++) {
            graph->matrix[i][j] = dist[i][j];
        }
    }
}

void print_shortest_paths(struct Graph* graph) {
    if (graph == NULL) {
        return;
    }
    printf("Shortest path distances:\n");
    for (int i = 0; i < graph->nodes; i++) {
        for (int j = 0; j < graph->nodes; j++) {
            if (graph->matrix[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", graph->matrix[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    struct Graph* graph = create_graph(4);
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    add_edge(graph, 0, 1, 5);
    add_edge(graph, 0, 3, 10);
    add_edge(graph, 1, 2, 3);
    add_edge(graph, 2, 3, 1);
    add_edge(graph, 3, 1, 2);
    printf("Original graph edges added.\n");
    floyd_warshall(graph);
    print_shortest_paths(graph);
    free(graph);
    return 0;
}