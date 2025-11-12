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

int add_edge(struct Graph* graph, int u, int v, int weight) {
    if (graph == NULL || u < 0 || u >= graph->nodes || v < 0 || v >= graph->nodes) {
        return 0;
    }
    if (graph->edge_count >= MAX_EDGES) {
        return 0;
    }
    graph->edges[graph->edge_count][0] = u;
    graph->edges[graph->edge_count][1] = v;
    graph->edges[graph->edge_count][2] = weight;
    graph->edge_count++;
    return 1;
}

void dijkstra(struct Graph* graph, int source) {
    if (graph == NULL || source < 0 || source >= graph->nodes) {
        return;
    }
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    for (int i = 0; i < graph->nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[source] = 0;
    for (int count = 0; count < graph->nodes - 1; count++) {
        int min_dist = INT_MAX;
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
        for (int i = 0; i < graph->edge_count; i++) {
            int u = graph->edges[i][0];
            int v = graph->edges[i][1];
            int weight = graph->edges[i][2];
            if (u == min_node && !visited[v] && dist[u] != INT_MAX) {
                long new_dist = (long)dist[u] + weight;
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = new_dist;
                }
            }
        }
    }
    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < graph->nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
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
    add_edge(graph, 0, 1, 10);
    add_edge(graph, 0, 2, 5);
    add_edge(graph, 1, 2, 2);
    add_edge(graph, 1, 3, 1);
    add_edge(graph, 2, 1, 3);
    add_edge(graph, 2, 3, 9);
    add_edge(graph, 2, 4, 2);
    add_edge(graph, 3, 4, 4);
    add_edge(graph, 4, 3, 6);
    printf("Graph with %d nodes and %d edges created.\n", graph->nodes, graph->edge_count);
    dijkstra(graph, 0);
    free(graph);
    return 0;
}