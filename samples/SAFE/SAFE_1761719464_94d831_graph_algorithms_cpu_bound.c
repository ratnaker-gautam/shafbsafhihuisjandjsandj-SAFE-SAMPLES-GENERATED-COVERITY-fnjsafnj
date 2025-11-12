//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

void initialize_graph(Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    g->node_count = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid edge nodes\n");
        exit(1);
    }
    g->adjacency[from][to] = weight;
    g->adjacency[to][from] = weight;
}

int find_min_distance(int distances[], int visited[], int node_count) {
    int min = INT_MAX;
    int min_index = -1;
    for (int v = 0; v < node_count; v++) {
        if (visited[v] == 0 && distances[v] <= min) {
            min = distances[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(Graph *g, int source, int distances[]) {
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
            if (!visited[v] && g->adjacency[u][v] != 0 && 
                distances[u] != INT_MAX && 
                distances[u] + g->adjacency[u][v] < distances[v]) {
                distances[v] = distances[u] + g->adjacency[u][v];
            }
        }
    }
}

void generate_sample_graph(Graph *g) {
    initialize_graph(g, 6);
    add_edge(g, 0, 1, 4);
    add_edge(g, 0, 2, 2);
    add_edge(g, 1, 2, 1);
    add_edge(g, 1, 3, 5);
    add_edge(g, 2, 3, 8);
    add_edge(g, 2, 4, 10);
    add_edge(g, 3, 4, 2);
    add_edge(g, 3, 5, 6);
    add_edge(g, 4, 5, 3);
}

int main() {
    Graph g;
    generate_sample_graph(&g);
    int distances[MAX_NODES];
    int source_node = 0;
    if (source_node < 0 || source_node >= g.node_count) {
        fprintf(stderr, "Invalid source node\n");
        return 1;
    }
    dijkstra(&g, source_node, distances);
    printf("Shortest distances from node %d:\n", source_node);
    for (int i = 0; i < g.node_count; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
    return 0;
}