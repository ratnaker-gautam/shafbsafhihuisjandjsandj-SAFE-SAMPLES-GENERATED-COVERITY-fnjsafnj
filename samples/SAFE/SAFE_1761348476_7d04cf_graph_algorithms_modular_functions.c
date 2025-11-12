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

int add_edge(struct Graph* graph, int u, int v, int weight) {
    if (graph == NULL || u < 0 || u >= graph->nodes || v < 0 || v >= graph->nodes) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    graph->adj[u][v] = weight;
    graph->adj[v][u] = weight;
    return 1;
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
        int min = INF;
        int min_index = -1;
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                min_index = v;
            }
        }
        if (min_index == -1) {
            break;
        }
        visited[min_index] = 1;
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && graph->adj[min_index][v] != INF && dist[min_index] != INF) {
                long new_dist = (long)dist[min_index] + graph->adj[min_index][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
    printf("Vertex Distance from Source %d\n", src);
    for (int i = 0; i < graph->nodes; i++) {
        if (dist[i] == INF) {
            printf("%d \t\t INF\n", i);
        } else {
            printf("%d \t\t %d\n", i, dist[i]);
        }
    }
}

int get_valid_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter integer between %d and %d.\n", min, max);
    }
}

int main() {
    printf("Graph Dijkstra Algorithm Demo\n");
    int nodes = get_valid_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    struct Graph* graph = create_graph(nodes);
    if (graph == NULL) {
        printf("Failed to create graph.\n");
        return 1;
    }
    int edges = get_valid_int("Enter number of edges: ", 0, nodes * (nodes - 1) / 2);
    for (int i = 0; i < edges; i++) {
        printf("Edge %d:\n", i + 1);
        int u = get_valid_int("Enter source node: ", 0, nodes - 1);
        int v = get_valid_int("Enter destination node: ", 0, nodes - 1);
        int weight = get_valid_int("Enter weight (0-10000): ", 0, 10000);
        if (!add_edge(graph, u, v, weight)) {
            printf("Failed to add edge.\n");
            i--;
        }
    }
    int src = get_valid_int("Enter source vertex for Dijkstra: ", 0, nodes - 1);
    dijkstra(graph, src);
    free(graph);
    return 0;
}