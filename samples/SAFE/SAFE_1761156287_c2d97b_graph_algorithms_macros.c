//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int graph[MAX_NODES][MAX_NODES];
int dist[MAX_NODES];
int visited[MAX_NODES];

void initialize_graph(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            graph[i][j] = (i == j) ? 0 : INF;
        }
    }
}

int read_graph(int n, int m) {
    for (int i = 0; i < m; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) return 0;
        if (u < 0 || u >= n || v < 0 || v >= n) return 0;
        if (w < 0) return 0;
        graph[u][v] = w;
        graph[v][u] = w;
    }
    return 1;
}

void dijkstra(int start, int n) {
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[start] = 0;

    for (int count = 0; count < n; count++) {
        int min_dist = INF;
        int min_node = -1;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                min_node = i;
            }
        }
        if (min_node == -1) break;
        visited[min_node] = 1;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && graph[min_node][i] != INF && dist[min_node] != INF) {
                if (graph[min_node][i] > INT_MAX - dist[min_node]) continue;
                int new_dist = dist[min_node] + graph[min_node][i];
                if (new_dist < dist[i]) {
                    dist[i] = new_dist;
                }
            }
        }
    }
}

int main() {
    int n, m, start;
    if (scanf("%d %d %d", &n, &m, &start) != 3) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    if (n <= 0 || n > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    if (m < 0 || m > n * (n - 1) / 2) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    if (start < 0 || start >= n) {
        fprintf(stderr, "Invalid start node\n");
        return 1;
    }

    initialize_graph(n);
    if (!read_graph(n, m)) {
        fprintf(stderr, "Invalid edge data\n");
        return 1;
    }

    dijkstra(start, n);

    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < n; i++) {
        if (dist[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }

    return 0;
}