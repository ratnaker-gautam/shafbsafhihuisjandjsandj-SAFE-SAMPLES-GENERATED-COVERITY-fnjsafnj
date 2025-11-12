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
        int min_distance = INF;
        int min_index = -1;
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && distances[v] <= min_distance) {
                min_distance = distances[v];
                min_index = v;
            }
        }
        if (min_index == -1) {
            break;
        }
        visited[min_index] = 1;
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && graph->matrix[min_index][v] != INF && distances[min_index] != INF) {
                long new_dist = (long)distances[min_index] + graph->matrix[min_index][v];
                if (new_dist < distances[v] && new_dist <= INT_MAX) {
                    distances[v] = (int)new_dist;
                }
            }
        }
    }
}

void print_distances(struct Graph* graph, int source, int distances[]) {
    if (graph == NULL || distances == NULL || source < 0 || source >= graph->nodes) {
        return;
    }
    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < graph->nodes; i++) {
        if (distances[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

int main() {
    int nodes;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (nodes <= 0 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    struct Graph* graph = create_graph(nodes);
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    int edges;
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        printf("Invalid input\n");
        free(graph);
        return 1;
    }
    if (edges < 0 || edges > nodes * nodes) {
        printf("Invalid number of edges\n");
        free(graph);
        return 1;
    }
    printf("Enter edges (from to weight):\n");
    for (int i = 0; i < edges; i++) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid edge input\n");
            free(graph);
            return 1;
        }
        if (!add_edge(graph, from, to, weight)) {
            printf("Invalid edge\n");
            free(graph);
            return 1;
        }
    }
    int source;
    printf("Enter source node: ");
    if (scanf("%d", &source) != 1) {
        printf("Invalid input\n");
        free(graph);
        return 1;
    }
    if (source < 0 || source >= nodes) {
        printf("Invalid source node\n");
        free(graph);
        return 1;
    }
    int distances[MAX_NODES];
    dijkstra(graph, source, distances);
    print_distances(graph, source, distances);
    free(graph);
    return 0;
}