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

void dijkstra(struct Graph* graph, int start) {
    if (graph == NULL || start < 0 || start >= graph->nodes) {
        return;
    }
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    for (int i = 0; i < graph->nodes; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[start] = 0;
    for (int count = 0; count < graph->nodes - 1; count++) {
        int min_dist = INF;
        int min_node = -1;
        for (int i = 0; i < graph->nodes; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                min_node = i;
            }
        }
        if (min_node == -1) {
            break;
        }
        visited[min_node] = 1;
        for (int i = 0; i < graph->nodes; i++) {
            if (!visited[i] && graph->adj[min_node][i] != INF && dist[min_node] != INF) {
                long new_dist = (long)dist[min_node] + graph->adj[min_node][i];
                if (new_dist < dist[i] && new_dist <= INT_MAX) {
                    dist[i] = (int)new_dist;
                }
            }
        }
    }
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < graph->nodes; i++) {
        if (dist[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int validate_input(const char* prompt, int min, int max) {
    printf("%s", prompt);
    int value;
    if (scanf("%d", &value) != 1) {
        while (getchar() != '\n');
        return -1;
    }
    if (value < min || value > max) {
        return -1;
    }
    return value;
}

int main() {
    int nodes = validate_input("Enter number of nodes (1-100): ", 1, MAX_NODES);
    if (nodes == -1) {
        printf("Invalid number of nodes.\n");
        return 1;
    }
    struct Graph* graph = create_graph(nodes);
    if (graph == NULL) {
        printf("Failed to create graph.\n");
        return 1;
    }
    int edges = validate_input("Enter number of edges: ", 0, nodes * (nodes - 1) / 2);
    if (edges == -1) {
        printf("Invalid number of edges.\n");
        free(graph);
        return 1;
    }
    for (int i = 0; i < edges; i++) {
        printf("Edge %d:\n", i + 1);
        int src = validate_input("  Source node: ", 0, nodes - 1);
        int dest = validate_input("  Destination node: ", 0, nodes - 1);
        int weight = validate_input("  Weight: ", 0, 1000);
        if (src == -1 || dest == -1 || weight == -1) {
            printf("Invalid edge data.\n");
            free(graph);
            return 1;
        }
        if (!add_edge(graph, src, dest, weight)) {
            printf("Failed to add edge.\n");
            free(graph);
            return 1;
        }
    }
    int start = validate_input("Enter starting node for Dijkstra: ", 0, nodes - 1);
    if (start == -1) {
        printf("Invalid starting node.\n");
        free(graph);
        return 1;
    }
    dijkstra(graph, start);
    free(graph);
    return 0;
}