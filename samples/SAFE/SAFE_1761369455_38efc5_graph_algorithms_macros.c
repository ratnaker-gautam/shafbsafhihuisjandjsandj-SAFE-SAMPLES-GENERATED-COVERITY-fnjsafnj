//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int graph[MAX_NODES][MAX_NODES];
int dist[MAX_NODES][MAX_NODES];
int node_count;

void initialize_graph() {
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            graph[i][j] = (i == j) ? 0 : INF;
            dist[i][j] = (i == j) ? 0 : INF;
        }
    }
}

int validate_node(int node) {
    return node >= 0 && node < node_count;
}

void add_edge(int u, int v, int weight) {
    if (!validate_node(u) || !validate_node(v)) {
        fprintf(stderr, "Invalid node index\n");
        return;
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        return;
    }
    graph[u][v] = weight;
    dist[u][v] = weight;
}

void floyd_warshall() {
    for (int k = 0; k < node_count; k++) {
        for (int i = 0; i < node_count; i++) {
            for (int j = 0; j < node_count; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = MIN(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
}

void print_shortest_paths() {
    printf("Shortest path distances:\n");
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
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
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    if (node_count < 1 || node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        return 1;
    }

    initialize_graph();

    printf("Enter edges (u v weight), -1 to stop:\n");
    int u, v, weight;
    while (1) {
        if (scanf("%d", &u) != 1) {
            fprintf(stderr, "Invalid input\n");
            return 1;
        }
        if (u == -1) break;
        if (scanf("%d %d", &v, &weight) != 2) {
            fprintf(stderr, "Invalid input\n");
            return 1;
        }
        add_edge(u, v, weight);
    }

    floyd_warshall();
    print_shortest_paths();

    return 0;
}