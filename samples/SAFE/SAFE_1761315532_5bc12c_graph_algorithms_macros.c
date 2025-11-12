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

int read_input() {
    int edge_count;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1 || node_count < 1 || node_count > MAX_NODES) {
        printf("Invalid node count\n");
        return 0;
    }
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1 || edge_count < 0) {
        printf("Invalid edge count\n");
        return 0;
    }
    
    printf("Enter edges as 'u v w' (0-indexed nodes, weight):\n");
    for (int i = 0; i < edge_count; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            printf("Invalid edge format\n");
            return 0;
        }
        if (u < 0 || u >= node_count || v < 0 || v >= node_count) {
            printf("Invalid node index\n");
            return 0;
        }
        if (w < 0) {
            printf("Negative weights not allowed\n");
            return 0;
        }
        graph[u][v] = w;
    }
    return 1;
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
                    dist[i][j] = MIN(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
}

void print_shortest_paths() {
    printf("\nShortest path distances:\n");
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
    
    if (!read_input()) {
        return 1;
    }
    
    floyd_warshall();
    print_shortest_paths();
    
    return 0;
}