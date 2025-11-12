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

void print_graph(struct Graph* graph) {
    if (graph == NULL) {
        return;
    }
    printf("Graph adjacency matrix:\n");
    for (int i = 0; i < graph->nodes; i++) {
        for (int j = 0; j < graph->nodes; j++) {
            if (graph->adj[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", graph->adj[i][j]);
            }
        }
        printf("\n");
    }
}

int find_min_distance(int dist[], int visited[], int nodes) {
    int min = INF;
    int min_index = -1;
    for (int v = 0; v < nodes; v++) {
        if (visited[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(struct Graph* graph, int src) {
    if (graph == NULL || src < 0 || src >= graph->nodes) {
        printf("Invalid input for Dijkstra\n");
        return;
    }
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    for (int i = 0; i < graph->nodes; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[src] = 0;
    for (int count = 0; count < graph->nodes - 1; count++) {
        int u = find_min_distance(dist, visited, graph->nodes);
        if (u == -1) {
            break;
        }
        visited[u] = 1;
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && graph->adj[u][v] != INF && dist[u] != INF && 
                dist[u] + graph->adj[u][v] < dist[v]) {
                dist[v] = dist[u] + graph->adj[u][v];
            }
        }
    }
    printf("Dijkstra shortest paths from node %d:\n", src);
    for (int i = 0; i < graph->nodes; i++) {
        if (dist[i] == INF) {
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
    add_edge(graph, 0, 1, 4);
    add_edge(graph, 0, 2, 1);
    add_edge(graph, 1, 2, 2);
    add_edge(graph, 1, 3, 5);
    add_edge(graph, 2, 3, 8);
    add_edge(graph, 2, 4, 10);
    add_edge(graph, 3, 4, 2);
    print_graph(graph);
    dijkstra(graph, 0);
    free(graph);
    return 0;
}