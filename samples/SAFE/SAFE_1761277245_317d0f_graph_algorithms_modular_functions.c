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

void print_graph(struct Graph* graph) {
    if (graph == NULL) {
        return;
    }
    printf("Graph adjacency matrix:\n");
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

void dijkstra(struct Graph* graph, int source) {
    if (graph == NULL || source < 0 || source >= graph->nodes) {
        return;
    }
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    for (int i = 0; i < graph->nodes; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[source] = 0;
    for (int count = 0; count < graph->nodes - 1; count++) {
        int min_dist = INF;
        int min_node = -1;
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_node = v;
            }
        }
        if (min_node == -1) {
            break;
        }
        visited[min_node] = 1;
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && graph->matrix[min_node][v] != INF && dist[min_node] != INF) {
                long new_dist = (long)dist[min_node] + graph->matrix[min_node][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < graph->nodes; i++) {
        if (dist[i] == INF) {
            printf("Node %d: INF\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph* graph = create_graph(5);
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    add_edge(graph, 0, 1, 4);
    add_edge(graph, 0, 2, 1);
    add_edge(graph, 1, 3, 1);
    add_edge(graph, 2, 1, 2);
    add_edge(graph, 2, 3, 5);
    add_edge(graph, 3, 4, 3);
    print_graph(graph);
    printf("\n");
    dijkstra(graph, 0);
    free(graph);
    return 0;
}