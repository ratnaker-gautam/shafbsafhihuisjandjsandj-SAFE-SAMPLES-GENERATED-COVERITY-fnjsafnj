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

int read_int() {
    int value;
    while (scanf("%d", &value) != 1) {
        printf("Invalid input. Enter an integer: ");
        while (getchar() != '\n');
    }
    return value;
}

int main() {
    int n, m;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    n = read_int();
    if (n < 1 || n > MAX_NODES) {
        printf("Number of nodes must be between 1 and %d.\n", MAX_NODES);
        return 1;
    }

    initialize_graph(n);

    printf("Enter number of edges: ");
    m = read_int();
    if (m < 0) {
        printf("Number of edges cannot be negative.\n");
        return 1;
    }

    printf("Enter edges as 'u v w' (0-indexed nodes, weight):\n");
    for (int i = 0; i < m; i++) {
        int u, v, w;
        u = read_int();
        v = read_int();
        w = read_int();

        if (u < 0 || u >= n || v < 0 || v >= n) {
            printf("Invalid node index. Must be between 0 and %d.\n", n-1);
            return 1;
        }
        if (w < 0) {
            printf("Weight must be non-negative.\n");
            return 1;
        }
        graph[u][v] = w;
    }

    floyd_warshall(n);

    printf("\nShortest path distances:\n");
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

    printf("\nEnter source and target nodes to find path (or -1 -1 to exit):\n");
    while (1) {
        int src, tgt;
        src = read_int();
        tgt = read_int();
        if (src == -1 && tgt == -1) break;
        
        if (src < 0 || src >= n || tgt < 0 || tgt >= n) {
            printf("Invalid nodes. Must be between 0 and %d.\n", n-1);
            continue;
        }
        
        if (dist[src][tgt] == INF) {
            printf("No path from %d to %d.\n", src, tgt);
        } else {
            printf("Shortest distance from %d to %d: %d\n", src, tgt, dist[src][tgt]);
        }
    }

    return 0;
}