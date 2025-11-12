//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int graph[MAX_NODES][MAX_NODES];
int dist[MAX_NODES][MAX_NODES];

void initialize_graph(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                graph[i][j] = 0;
            } else {
                graph[i][j] = INF;
            }
        }
    }
}

int read_input(int *n, int *m) {
    if (scanf("%d %d", n, m) != 2) {
        return 0;
    }
    if (*n <= 0 || *n > MAX_NODES || *m < 0 || *m > (*n * (*n - 1) / 2)) {
        return 0;
    }
    initialize_graph(*n);
    for (int i = 0; i < *m; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            return 0;
        }
        if (u < 0 || u >= *n || v < 0 || v >= *n || w < 0) {
            return 0;
        }
        graph[u][v] = w;
        graph[v][u] = w;
    }
    return 1;
}

void floyd_warshall(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dist[i][j] = graph[i][j];
        }
    }
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = MIN(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
}

void print_shortest_paths(int n) {
    printf("Shortest path distances:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (dist[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", dist[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    int n, m;
    printf("Enter number of nodes and edges: ");
    if (!read_input(&n, &m)) {
        printf("Invalid input\n");
        return 1;
    }
    floyd_warshall(n);
    print_shortest_paths(n);
    return 0;
}