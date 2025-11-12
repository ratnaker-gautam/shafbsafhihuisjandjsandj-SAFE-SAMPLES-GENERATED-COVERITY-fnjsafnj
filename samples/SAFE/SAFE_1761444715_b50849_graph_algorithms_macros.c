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

int read_int() {
    int value;
    if (scanf("%d", &value) != 1) {
        fprintf(stderr, "Invalid input\n");
        exit(EXIT_FAILURE);
    }
    return value;
}

void read_graph(int n, int m) {
    for (int i = 0; i < m; i++) {
        int u = read_int();
        int v = read_int();
        int w = read_int();
        if (u < 0 || u >= n || v < 0 || v >= n) {
            fprintf(stderr, "Invalid node\n");
            exit(EXIT_FAILURE);
        }
        if (w < 0) {
            fprintf(stderr, "Invalid weight\n");
            exit(EXIT_FAILURE);
        }
        graph[u][v] = w;
        graph[v][u] = w;
    }
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
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
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
    printf("Enter number of nodes: ");
    n = read_int();
    if (n <= 0 || n > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return EXIT_FAILURE;
    }
    printf("Enter number of edges: ");
    m = read_int();
    if (m < 0 || m > n * (n - 1) / 2) {
        fprintf(stderr, "Invalid number of edges\n");
        return EXIT_FAILURE;
    }
    initialize_graph(n);
    printf("Enter edges (u v w):\n");
    read_graph(n, m);
    floyd_warshall(n);
    printf("Shortest path distances:\n");
    print_shortest_paths(n);
    return EXIT_SUCCESS;
}