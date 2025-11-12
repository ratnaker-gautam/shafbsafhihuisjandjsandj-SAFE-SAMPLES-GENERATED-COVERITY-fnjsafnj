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
            if (i == j) {
                graph[i][j] = 0;
            } else {
                graph[i][j] = INF;
            }
        }
    }
}

int validate_node(int node) {
    return node >= 0 && node < node_count;
}

void add_edge(int u, int v, int weight) {
    if (!validate_node(u) || !validate_node(v)) {
        printf("Invalid node(s)\n");
        return;
    }
    if (weight < 0) {
        printf("Negative weights not allowed\n");
        return;
    }
    graph[u][v] = weight;
}

void floyd_warshall() {
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            dist[i][j] = graph[i][j];
        }
    }

    for (int k = 0; k < node_count; k++) {
        for (int i = 0; i < node_count; i++) {
            for (int j = 0; j < node_count; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
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
    initialize_graph();
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (node_count < 1 || node_count > MAX_NODES) {
        printf("Invalid node count\n");
        return 1;
    }

    int edge_count;
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (edge_count < 0 || edge_count > node_count * node_count) {
        printf("Invalid edge count\n");
        return 1;
    }

    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < edge_count; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        add_edge(u, v, w);
    }

    floyd_warshall();
    print_shortest_paths();

    return 0;
}