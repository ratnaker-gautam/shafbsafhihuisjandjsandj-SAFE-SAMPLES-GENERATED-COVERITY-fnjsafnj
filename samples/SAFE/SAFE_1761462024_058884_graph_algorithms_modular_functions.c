//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int nodes;
    int edges[MAX_EDGES][3];
    int edge_count;
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
    graph->edge_count = 0;
    return graph;
}

int add_edge(struct Graph* graph, int from, int to, int weight) {
    if (graph == NULL || from < 0 || from >= graph->nodes || to < 0 || to >= graph->nodes) {
        return 0;
    }
    if (graph->edge_count >= MAX_EDGES) {
        return 0;
    }
    graph->edges[graph->edge_count][0] = from;
    graph->edges[graph->edge_count][1] = to;
    graph->edges[graph->edge_count][2] = weight;
    graph->edge_count++;
    return 1;
}

void dijkstra(struct Graph* graph, int source, int* distances) {
    if (graph == NULL || distances == NULL || source < 0 || source >= graph->nodes) {
        return;
    }
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < graph->nodes; i++) {
        distances[i] = INT_MAX;
    }
    distances[source] = 0;
    for (int count = 0; count < graph->nodes - 1; count++) {
        int min_distance = INT_MAX;
        int min_node = -1;
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && distances[v] <= min_distance) {
                min_distance = distances[v];
                min_node = v;
            }
        }
        if (min_node == -1) {
            break;
        }
        visited[min_node] = 1;
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i][0] == min_node) {
                int to = graph->edges[i][1];
                int weight = graph->edges[i][2];
                if (!visited[to] && distances[min_node] != INT_MAX && distances[min_node] + weight < distances[to]) {
                    distances[to] = distances[min_node] + weight;
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
        if (distances[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

int main() {
    int nodes = 5;
    struct Graph* graph = create_graph(nodes);
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    add_edge(graph, 0, 1, 4);
    add_edge(graph, 0, 2, 1);
    add_edge(graph, 1, 2, 2);
    add_edge(graph, 1, 3, 5);
    add_edge(graph, 2, 1, 2);
    add_edge(graph, 2, 3, 8);
    add_edge(graph, 2, 4, 9);
    add_edge(graph, 3, 4, 6);
    add_edge(graph, 4, 3, 6);
    int distances[MAX_NODES];
    dijkstra(graph, 0, distances);
    print_distances(distances, nodes, 0);
    free(graph);
    return 0;
}