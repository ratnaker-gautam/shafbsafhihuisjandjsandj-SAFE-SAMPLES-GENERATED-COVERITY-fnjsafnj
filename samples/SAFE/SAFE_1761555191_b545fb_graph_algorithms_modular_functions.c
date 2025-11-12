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

struct Graph* create_graph(int n) {
    if (n <= 0 || n > MAX_NODES) {
        return NULL;
    }
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->nodes = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
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

int min_distance(int dist[], int visited[], int n) {
    int min = INF;
    int min_index = -1;
    for (int v = 0; v < n; v++) {
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
            if (!visited[v] && graph->adj[u][v] != INF && dist[u] != INF) {
                long new_dist = (long)dist[u] + graph->adj[u][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
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
            continue;
        }
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter integer between %d and %d.\n", min, max);
    }
}

int main() {
    printf("Graph Algorithms: Dijkstra's Shortest Path\n");
    int n = get_valid_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    struct Graph* graph = create_graph(n);
    if (graph == NULL) {
        printf("Failed to create graph.\n");
        return 1;
    }
    int edges = get_valid_int("Enter number of edges: ", 0, n * (n - 1) / 2);
    for (int i = 0; i < edges; i++) {
        printf("Edge %d:\n", i + 1);
        int u = get_valid_int("  Enter source node: ", 0, n - 1);
        int v = get_valid_int("  Enter destination node: ", 0, n - 1);
        int w = get_valid_int("  Enter weight (0-10000): ", 0, 10000);
        if (!add_edge(graph, u, v, w)) {
            printf("Failed to add edge.\n");
            i--;
        }
    }
    int src = get_valid_int("Enter source vertex for Dijkstra: ", 0, n - 1);
    dijkstra(graph, src);
    free(graph);
    return 0;
}