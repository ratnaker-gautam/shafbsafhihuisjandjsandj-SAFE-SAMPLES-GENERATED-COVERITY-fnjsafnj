//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

void initialize_graph(Graph *g, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid edge nodes\n");
        exit(1);
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weight not allowed\n");
        exit(1);
    }
    g->adjacency[from][to] = weight;
    g->adjacency[to][from] = weight;
}

int find_min_distance(int distances[], int visited[], int node_count) {
    int min_distance = INT_MAX;
    int min_index = -1;
    for (int i = 0; i < node_count; i++) {
        if (!visited[i] && distances[i] <= min_distance) {
            min_distance = distances[i];
            min_index = i;
        }
    }
    return min_index;
}

void dijkstra_shortest_path(Graph *g, int source, int distances[]) {
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < g->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[source] = 0;
    for (int count = 0; count < g->node_count - 1; count++) {
        int u = find_min_distance(distances, visited, g->node_count);
        if (u == -1) break;
        visited[u] = 1;
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && g->adjacency[u][v] != 0) {
                int new_distance = distances[u] + g->adjacency[u][v];
                if (new_distance > 0 && new_distance < distances[v]) {
                    distances[v] = new_distance;
                }
            }
        }
    }
}

int main() {
    Graph graph;
    int node_count;
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &node_count) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    initialize_graph(&graph, node_count);
    int edge_count;
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    printf("Enter edges as 'from to weight' (0-indexed):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&graph, from, to, weight);
    }
    int source;
    printf("Enter source node: ");
    if (scanf("%d", &source) != 1) {
        fprintf(stderr, "Invalid source node\n");
        return 1;
    }
    if (source < 0 || source >= node_count) {
        fprintf(stderr, "Invalid source node\n");
        return 1;
    }
    int distances[MAX_NODES];
    dijkstra_shortest_path(&graph, source, distances);
    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < node_count; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
    return 0;
}