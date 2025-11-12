//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
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

void dijkstra(struct Graph* graph, int source, int distances[]) {
    if (graph == NULL || distances == NULL || source < 0 || source >= graph->nodes) {
        return;
    }
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < graph->nodes; i++) {
        distances[i] = INF;
    }
    distances[source] = 0;
    for (int count = 0; count < graph->nodes - 1; count++) {
        int min_dist = INF;
        int min_node = -1;
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && distances[v] <= min_dist) {
                min_dist = distances[v];
                min_node = v;
            }
        }
        if (min_node == -1) {
            break;
        }
        visited[min_node] = 1;
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && graph->matrix[min_node][v] != INF && distances[min_node] != INF) {
                long new_dist = (long)distances[min_node] + graph->matrix[min_node][v];
                if (new_dist < distances[v] && new_dist <= INT_MAX) {
                    distances[v] = (int)new_dist;
                }
            }
        }
    }
}

void print_distances(int distances[], int nodes, int source) {
    if (distances == NULL || nodes <= 0 || nodes > MAX_NODES || source < 0 || source >= nodes) {
        return;
    }
    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < nodes; i++) {
        if (distances[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

int main() {
    int nodes = 6;
    struct Graph* graph = create_graph(nodes);
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    add_edge(graph, 0, 1, 4);
    add_edge(graph, 0, 2, 2);
    add_edge(graph, 1, 2, 1);
    add_edge(graph, 1, 3, 5);
    add_edge(graph, 2, 3, 8);
    add_edge(graph, 2, 4, 10);
    add_edge(graph, 3, 4, 2);
    add_edge(graph, 3, 5, 6);
    add_edge(graph, 4, 5, 3);
    int distances[MAX_NODES];
    int source = 0;
    dijkstra(graph, source, distances);
    print_distances(distances, nodes, source);
    free(graph);
    return 0;
}