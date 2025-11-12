//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int edges[MAX_NODES][MAX_NODES];
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
            g->edges[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid edge\n");
        exit(1);
    }
    g->edges[from][to] = weight;
    g->edges[to][from] = weight;
}

int find_min_distance(int distances[], int visited[], int n) {
    int min = INT_MAX;
    int min_index = -1;
    int i = 0;
    while (i < n) {
        if (!visited[i] && distances[i] <= min) {
            min = distances[i];
            min_index = i;
        }
        i++;
    }
    return min_index;
}

void dijkstra_shortest_path(Graph *g, int start) {
    int distances[MAX_NODES];
    int visited[MAX_NODES];
    int i = 0;
    while (i < g->node_count) {
        distances[i] = INT_MAX;
        visited[i] = 0;
        i++;
    }
    distances[start] = 0;
    int count = 0;
    while (count < g->node_count - 1) {
        int u = find_min_distance(distances, visited, g->node_count);
        if (u == -1) break;
        visited[u] = 1;
        int v = 0;
        do {
            if (!visited[v] && g->edges[u][v] && distances[u] != INT_MAX) {
                int new_dist = distances[u] + g->edges[u][v];
                if (new_dist > 0 && new_dist < distances[v]) {
                    distances[v] = new_dist;
                }
            }
            v++;
        } while (v < g->node_count);
        count++;
    }
    printf("Shortest distances from node %d:\n", start);
    for (int j = 0; j < g->node_count; j++) {
        if (distances[j] == INT_MAX) {
            printf("Node %d: unreachable\n", j);
        } else {
            printf("Node %d: %d\n", j, distances[j]);
        }
    }
}

void depth_first_search(Graph *g, int node, int visited[]) {
    visited[node] = 1;
    printf("Visited node %d\n", node);
    int i = 0;
    for (; i < g->node_count; i++) {
        if (g->edges[node][i] && !visited[i]) {
            depth_first_search(g, i, visited);
        }
    }
}

int main() {
    Graph graph;
    int nodes = 6;
    initialize_graph(&graph, nodes);
    add_edge(&graph, 0, 1, 4);
    add_edge(&graph, 0, 2, 2);
    add_edge(&graph, 1, 2, 1);
    add_edge(&graph, 1, 3, 5);
    add_edge(&graph, 2, 3, 8);
    add_edge(&graph, 2, 4, 10);
    add_edge(&graph, 3, 4, 2);
    add_edge(&graph, 3, 5, 6);
    add_edge(&graph, 4, 5, 3);
    printf("Dijkstra's algorithm:\n");
    dijkstra_shortest_path(&graph, 0);
    printf("\nDepth-first search from node 0:\n");
    int visited[MAX_NODES] = {0};
    depth_first_search(&graph, 0, visited);
    return 0;
}