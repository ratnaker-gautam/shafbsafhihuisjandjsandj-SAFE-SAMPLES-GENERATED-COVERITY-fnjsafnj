//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
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

int min_distance(int dist[], int visited[], int nodes) {
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
        int u = min_distance(dist, visited, graph->nodes);
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
    printf("Vertex Distance from Source %d\n", src);
    for (int i = 0; i < graph->nodes; i++) {
        if (dist[i] == INF) {
            printf("%d \t INF\n", i);
        } else {
            printf("%d \t %d\n", i, dist[i]);
        }
    }
}

int get_valid_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter integer between %d and %d.\n", min, max);
    }
}

int main() {
    int nodes = get_valid_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    if (nodes == -1) {
        return 1;
    }
    struct Graph* graph = create_graph(nodes);
    if (graph == NULL) {
        printf("Failed to create graph.\n");
        return 1;
    }
    int edges = get_valid_int("Enter number of edges: ", 0, nodes * (nodes - 1) / 2);
    if (edges == -1) {
        free(graph);
        return 1;
    }
    for (int i = 0; i < edges; i++) {
        printf("Edge %d:\n", i + 1);
        int src = get_valid_int("  Source node: ", 0, nodes - 1);
        int dest = get_valid_int("  Destination node: ", 0, nodes - 1);
        int weight = get_valid_int("  Weight: ", 0, 1000);
        if (src == -1 || dest == -1 || weight == -1) {
            free(graph);
            return 1;
        }
        if (!add_edge(graph, src, dest, weight)) {
            printf("Failed to add edge.\n");
            free(graph);
            return 1;
        }
    }
    int start_node = get_valid_int("Enter starting node for Dijkstra: ", 0, nodes - 1);
    if (start_node == -1) {
        free(graph);
        return 1;
    }
    dijkstra(graph, start_node);
    free(graph);
    return 0;
}