//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int num_nodes;
    int num_edges;
    int adjacency[MAX_NODES][MAX_NODES];
    int edge_weights[MAX_NODES][MAX_NODES];
};

struct Graph* create_graph(int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        return NULL;
    }
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (!graph) {
        return NULL;
    }
    graph->num_nodes = nodes;
    graph->num_edges = 0;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            graph->adjacency[i][j] = 0;
            graph->edge_weights[i][j] = 0;
        }
    }
    return graph;
}

int add_edge(struct Graph* graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->num_nodes || dest < 0 || dest >= graph->num_nodes) {
        return 0;
    }
    if (graph->adjacency[src][dest] != 0) {
        return 0;
    }
    graph->adjacency[src][dest] = 1;
    graph->edge_weights[src][dest] = weight;
    graph->num_edges++;
    return 1;
}

void dijkstra(struct Graph* graph, int start_node) {
    if (!graph || start_node < 0 || start_node >= graph->num_nodes) {
        return;
    }
    int distances[MAX_NODES];
    int visited[MAX_NODES];
    for (int i = 0; i < graph->num_nodes; i++) {
        distances[i] = INT_MAX;
        visited[i] = 0;
    }
    distances[start_node] = 0;
    for (int count = 0; count < graph->num_nodes - 1; count++) {
        int min_distance = INT_MAX;
        int min_index = -1;
        for (int v = 0; v < graph->num_nodes; v++) {
            if (!visited[v] && distances[v] <= min_distance) {
                min_distance = distances[v];
                min_index = v;
            }
        }
        if (min_index == -1) {
            break;
        }
        visited[min_index] = 1;
        for (int v = 0; v < graph->num_nodes; v++) {
            if (!visited[v] && graph->adjacency[min_index][v] && distances[min_index] != INT_MAX) {
                long new_dist = (long)distances[min_index] + graph->edge_weights[min_index][v];
                if (new_dist < distances[v] && new_dist <= INT_MAX) {
                    distances[v] = (int)new_dist;
                }
            }
        }
    }
    printf("Shortest distances from node %d:\n", start_node);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

void print_graph(struct Graph* graph) {
    if (!graph) {
        return;
    }
    printf("Graph with %d nodes and %d edges:\n", graph->num_nodes, graph->num_edges);
    for (int i = 0; i < graph->num_nodes; i++) {
        for (int j = 0; j < graph->num_nodes; j++) {
            if (graph->adjacency[i][j]) {
                printf("Edge %d -> %d (weight %d)\n", i, j, graph->edge_weights[i][j]);
            }
        }
    }
}

int main() {
    struct Graph* graph = create_graph(6);
    if (!graph) {
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
    print_graph(graph);
    printf("\n");
    dijkstra(graph, 0);
    free(graph);
    return 0;
}