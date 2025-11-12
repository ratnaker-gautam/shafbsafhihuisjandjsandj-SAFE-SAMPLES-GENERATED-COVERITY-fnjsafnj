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
            if (!visited[v] && dist[v] <= min_dist) {
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

void print_distances(int dist[], int n, int src) {
    if (dist == NULL || n <= 0 || src < 0 || src >= n) {
        return;
    }
    printf("Shortest distances from node %d:\n", src);
    for (int i = 0; i < n; i++) {
        if (dist[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    int nodes;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (nodes <= 0 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    struct Graph* graph = create_graph(nodes);
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    int edges;
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        printf("Invalid input\n");
        free(graph);
        return 1;
    }
    if (edges < 0 || edges > nodes * (nodes - 1) / 2) {
        printf("Invalid number of edges\n");
        free(graph);
        return 1;
    }
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            printf("Invalid edge input\n");
            free(graph);
            return 1;
        }
        if (!add_edge(graph, u, v, weight)) {
            printf("Invalid edge\n");
            free(graph);
            return 1;
        }
    }
    int src;
    printf("Enter source node: ");
    if (scanf("%d", &src) != 1) {
        printf("Invalid input\n");
        free(graph);
        return 1;
    }
    if (src < 0 || src >= nodes) {
        printf("Invalid source node\n");
        free(graph);
        return 1;
    }
    int dist[MAX_NODES];
    dijkstra(graph, src, dist);
    print_distances(dist, nodes, src);
    free(graph);
    return 0;
}