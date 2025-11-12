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
            graph->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
    return graph;
}

int add_edge(struct Graph* graph, int src, int dest, int weight) {
    if (graph == NULL || src < 0 || src >= graph->nodes || dest < 0 || dest >= graph->nodes) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    graph->adj[src][dest] = weight;
    graph->adj[dest][src] = weight;
    return 1;
}

void dijkstra(struct Graph* graph, int src, int dist[]) {
    if (graph == NULL || src < 0 || src >= graph->nodes || dist == NULL) {
        return;
    }
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < graph->nodes; i++) {
        dist[i] = INF;
    }
    dist[src] = 0;
    for (int count = 0; count < graph->nodes - 1; count++) {
        int min_dist = INF;
        int min_node = -1;
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && dist[v] < min_dist) {
                min_dist = dist[v];
                min_node = v;
            }
        }
        if (min_node == -1) {
            break;
        }
        visited[min_node] = 1;
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && graph->adj[min_node][v] != INF && dist[min_node] != INF) {
                long new_dist = (long)dist[min_node] + graph->adj[min_node][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
}

void print_distances(struct Graph* graph, int src, int dist[]) {
    if (graph == NULL || src < 0 || src >= graph->nodes || dist == NULL) {
        return;
    }
    printf("Shortest distances from node %d:\n", src);
    for (int i = 0; i < graph->nodes; i++) {
        if (dist[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    int nodes, edges;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes.\n");
        return 1;
    }
    struct Graph* graph = create_graph(nodes);
    if (graph == NULL) {
        printf("Failed to create graph.\n");
        return 1;
    }
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        printf("Invalid number of edges.\n");
        free(graph);
        return 1;
    }
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input.\n");
            free(graph);
            return 1;
        }
        if (!add_edge(graph, src, dest, weight)) {
            printf("Invalid edge parameters.\n");
            free(graph);
            return 1;
        }
    }
    int src;
    printf("Enter source node for Dijkstra: ");
    if (scanf("%d", &src) != 1 || src < 0 || src >= nodes) {
        printf("Invalid source node.\n");
        free(graph);
        return 1;
    }
    int dist[MAX_NODES];
    dijkstra(graph, src, dist);
    print_distances(graph, src, dist);
    free(graph);
    return 0;
}