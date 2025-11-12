//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct Graph {
    int nodes;
    int matrix[MAX_NODES][MAX_NODES];
};

struct Graph* create_graph(int n) {
    if (n <= 0 || n > MAX_NODES) {
        return NULL;
    }
    struct Graph* g = malloc(sizeof(struct Graph));
    if (g == NULL) {
        return NULL;
    }
    g->nodes = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g->matrix[i][j] = (i == j) ? 0 : INF;
        }
    }
    return g;
}

int add_edge(struct Graph* g, int u, int v, int w) {
    if (g == NULL || u < 0 || u >= g->nodes || v < 0 || v >= g->nodes || w < 0) {
        return 0;
    }
    g->matrix[u][v] = w;
    g->matrix[v][u] = w;
    return 1;
}

void dijkstra(struct Graph* g, int src, int dist[]) {
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
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_node = v;
            }
        }
        if (min_node == -1) {
            break;
        }
        visited[min_node] = 1;
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && g->matrix[min_node][v] != INF && dist[min_node] != INF) {
                long new_dist = (long)dist[min_node] + g->matrix[min_node][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
}

void print_distances(struct Graph* g, int src, int dist[]) {
    if (g == NULL || src < 0 || src >= g->nodes || dist == NULL) {
        return;
    }
    printf("Shortest distances from node %d:\n", src);
    for (int i = 0; i < g->nodes; i++) {
        if (dist[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    int nodes, edges;
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    struct Graph* g = create_graph(nodes);
    if (g == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        printf("Invalid number of edges\n");
        free(g);
        return 1;
    }
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            printf("Invalid edge input\n");
            free(g);
            return 1;
        }
        if (!add_edge(g, u, v, w)) {
            printf("Invalid edge parameters\n");
            free(g);
            return 1;
        }
    }
    int src;
    printf("Enter source node: ");
    if (scanf("%d", &src) != 1 || src < 0 || src >= nodes) {
        printf("Invalid source node\n");
        free(g);
        return 1;
    }
    int dist[MAX_NODES];
    dijkstra(g, src, dist);
    print_distances(g, src, dist);
    free(g);
    return 0;
}