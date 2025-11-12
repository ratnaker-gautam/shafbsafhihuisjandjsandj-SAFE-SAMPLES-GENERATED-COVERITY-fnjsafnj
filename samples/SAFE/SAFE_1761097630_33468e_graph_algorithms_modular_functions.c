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

void initialize_graph(struct Graph *g, int nodes) {
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES) {
        return;
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
}

int add_edge(struct Graph *g, int u, int v, int weight) {
    if (g == NULL || u < 0 || u >= g->nodes || v < 0 || v >= g->nodes || weight < 0) {
        return 0;
    }
    g->adj[u][v] = weight;
    g->adj[v][u] = weight;
    return 1;
}

void dijkstra(struct Graph *g, int src, int dist[]) {
    if (g == NULL || src < 0 || src >= g->nodes || dist == NULL) {
        return;
    }
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INF;
    }
    dist[src] = 0;
    for (int count = 0; count < g->nodes - 1; count++) {
        int min_dist = INF;
        int min_node = -1;
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && dist[v] < min_dist) {
                min_dist = dist[v];
                min_node = v;
            }
        }
        if (min_node == -1) {
            break;
        }
        visited[min_node] = 1;
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && g->adj[min_node][v] != INF && dist[min_node] != INF) {
                long long new_dist = (long long)dist[min_node] + g->adj[min_node][v];
                if (new_dist < dist[v] && new_dist <= INF) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
}

void print_shortest_paths(struct Graph *g, int src) {
    if (g == NULL || src < 0 || src >= g->nodes) {
        return;
    }
    int dist[MAX_NODES];
    dijkstra(g, src, dist);
    printf("Shortest paths from node %d:\n", src);
    for (int i = 0; i < g->nodes; i++) {
        if (dist[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: distance %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph g;
    int nodes = 6;
    initialize_graph(&g, nodes);
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 2);
    add_edge(&g, 1, 2, 1);
    add_edge(&g, 1, 3, 5);
    add_edge(&g, 2, 3, 8);
    add_edge(&g, 2, 4, 10);
    add_edge(&g, 3, 4, 2);
    add_edge(&g, 3, 5, 6);
    add_edge(&g, 4, 5, 3);
    printf("Graph with %d nodes created.\n", nodes);
    printf("Enter starting node (0-%d): ", nodes - 1);
    int start;
    if (scanf("%d", &start) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    if (start < 0 || start >= nodes) {
        printf("Invalid node.\n");
        return 1;
    }
    print_shortest_paths(&g, start);
    return 0;
}