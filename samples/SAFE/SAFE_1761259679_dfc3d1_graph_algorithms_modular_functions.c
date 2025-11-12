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
            if (!visited[v] && graph->adj[u][v] != INF && dist[u] != INF) {
                long new_dist = (long)dist[u] + graph->adj[u][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
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

int validate_integer_input(int min, int max) {
    int value;
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    if (sscanf(buffer, "%d", &value) != 1) {
        return -1;
    }
    if (value < min || value > max) {
        return -1;
    }
    return value;
}

int main() {
    printf("Graph Algorithm Demo\n");
    printf("Enter number of nodes (1-100): ");
    int nodes = validate_integer_input(1, MAX_NODES);
    if (nodes == -1) {
        printf("Invalid input\n");
        return 1;
    }
    struct Graph* graph = create_graph(nodes);
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    printf("Enter number of edges: ");
    int edges = validate_integer_input(0, nodes * (nodes - 1) / 2);
    if (edges == -1) {
        printf("Invalid input\n");
        free(graph);
        return 1;
    }
    for (int i = 0; i < edges; i++) {
        printf("Enter edge %d (src dest weight): ", i + 1);
        int src, dest, weight;
        char buffer[100];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            free(graph);
            return 1;
        }
        if (sscanf(buffer, "%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge format\n");
            free(graph);
            return 1;
        }
        if (!add_edge(graph, src, dest, weight)) {
            printf("Invalid edge parameters\n");
            free(graph);
            return 1;
        }
    }
    printf("Enter source node for Dijkstra (0-%d): ", nodes - 1);
    int src = validate_integer_input(0, nodes - 1);
    if (src == -1) {
        printf("Invalid source node\n");
        free(graph);
        return 1;
    }
    dijkstra(graph, src);
    free(graph);
    return 0;
}