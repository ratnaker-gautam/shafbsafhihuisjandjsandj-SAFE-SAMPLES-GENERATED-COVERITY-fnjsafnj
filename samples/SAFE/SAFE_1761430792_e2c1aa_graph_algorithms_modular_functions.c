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

void dijkstra(struct Graph* graph, int source, int* distances) {
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
        for (int i = 0; i < graph->nodes; i++) {
            if (!visited[i] && distances[i] < min_dist) {
                min_dist = distances[i];
                min_node = i;
            }
        }
        if (min_node == -1) {
            break;
        }
        visited[min_node] = 1;
        for (int i = 0; i < graph->nodes; i++) {
            if (!visited[i] && graph->matrix[min_node][i] != INF) {
                long new_dist = (long)distances[min_node] + graph->matrix[min_node][i];
                if (new_dist < distances[i] && new_dist <= INT_MAX) {
                    distances[i] = (int)new_dist;
                }
            }
        }
    }
}

void print_distances(int* distances, int nodes, int source) {
    if (distances == NULL || nodes <= 0 || source < 0 || source >= nodes) {
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
    int nodes;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (nodes < 1 || nodes > MAX_NODES) {
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
    for (int i = 0; i < edges; i++) {
        int from, to, weight;
        printf("Enter edge %d (from to weight): ", i + 1);
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid input\n");
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
    printf("Enter source node for Dijkstra: ");
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
    print_distances(distances, nodes, source);
    free(graph);
    return 0;
}