//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
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
    if (g == NULL || u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        return 0;
    }
    if (w < 0) {
        return 0;
    }
    g->matrix[u][v] = w;
    g->matrix[v][u] = w;
    return 1;
}

void dijkstra(struct Graph* g, int src) {
    if (g == NULL || src < 0 || src >= g->nodes) {
        return;
    }
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[src] = 0;
    for (int count = 0; count < g->nodes - 1; count++) {
        int min = INF;
        int u = -1;
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                u = v;
            }
        }
        if (u == -1) {
            break;
        }
        visited[u] = 1;
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && g->matrix[u][v] != INF && dist[u] != INF) {
                long sum = (long)dist[u] + g->matrix[u][v];
                if (sum < dist[v] && sum <= INT_MAX) {
                    dist[v] = (int)sum;
                }
            }
        }
    }
    printf("Vertex Distance from Source %d\n", src);
    for (int i = 0; i < g->nodes; i++) {
        if (dist[i] == INF) {
            printf("%d \t INF\n", i);
        } else {
            printf("%d \t %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph* g = create_graph(5);
    if (g == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    add_edge(g, 0, 1, 10);
    add_edge(g, 0, 2, 5);
    add_edge(g, 1, 2, 2);
    add_edge(g, 1, 3, 1);
    add_edge(g, 2, 3, 9);
    add_edge(g, 2, 4, 2);
    add_edge(g, 3, 4, 4);
    dijkstra(g, 0);
    free(g);
    return 0;
}