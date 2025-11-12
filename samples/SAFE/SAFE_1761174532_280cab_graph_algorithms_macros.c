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

int read_graph(int n, int m) {
    for (int i = 0; i < m; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            return 0;
        }
        if (u < 0 || u >= n || v < 0 || v >= n) {
            return 0;
        }
        if (w < 0) {
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
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (dist[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%d ", dist[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    if (n <= 0 || n > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    if (m < 0 || m > n * (n - 1) / 2) {
        printf("Invalid number of edges\n");
        return 1;
    }

    initialize_graph(n);
    if (!read_graph(n, m)) {
        printf("Invalid edge data\n");
        return 1;
    }

    floyd_warshall(n);
    print_shortest_paths(n);

    return 0;
}