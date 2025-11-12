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

int main() {
    int nodes;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    struct Graph* graph = create_graph(nodes);
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    int edges;
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        printf("Invalid input\n");
        free(graph);
        return 1;
    }
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        printf("Enter edge %d (src dest weight): ", i + 1);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid input\n");
            free(graph);
            return 1;
        }
        if (!add_edge(graph, src, dest, weight)) {
            printf("Invalid edge\n");
            free(graph);
            return 1;
        }
    }
    print_graph(graph);
    int start;
    printf("Enter starting node for Dijkstra: ");
    if (scanf("%d", &start) != 1) {
        printf("Invalid input\n");
        free(graph);
        return 1;
    }
    dijkstra(graph, start);
    free(graph);
    return 0;
}