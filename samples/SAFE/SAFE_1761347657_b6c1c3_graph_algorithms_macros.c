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

int read_int() {
    int value;
    if (scanf("%d", &value) != 1) {
        fprintf(stderr, "Invalid input\n");
        exit(EXIT_FAILURE);
    }
    return value;
}

void initialize_graph(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            graph[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void add_edge(int u, int v, int weight, int n) {
    if (u < 0 || u >= n || v < 0 || v >= n) {
        fprintf(stderr, "Invalid node index\n");
        exit(EXIT_FAILURE);
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        exit(EXIT_FAILURE);
    }
    graph[u][v] = weight;
    graph[v][u] = weight;
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
            if (!visited[i] && graph[min_node][i] != INF) {
                if (dist[min_node] < INF - graph[min_node][i]) {
                    int new_dist = dist[min_node] + graph[min_node][i];
                    if (new_dist < dist[i]) {
                        dist[i] = new_dist;
                    }
                }
            }
        }
    }
}

int main() {
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    int n = read_int();
    if (n < 1 || n > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return EXIT_FAILURE;
    }

    initialize_graph(n);

    printf("Enter number of edges: ");
    int m = read_int();
    if (m < 0 || m > n * (n - 1) / 2) {
        fprintf(stderr, "Invalid number of edges\n");
        return EXIT_FAILURE;
    }

    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < m; i++) {
        int u = read_int();
        int v = read_int();
        int weight = read_int();
        add_edge(u, v, weight, n);
    }

    printf("Enter start node: ");
    int start = read_int();
    if (start < 0 || start >= n) {
        fprintf(stderr, "Invalid start node\n");
        return EXIT_FAILURE;
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

    return EXIT_SUCCESS;
}