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
    if (scanf("%d", &node_count) != 1) {
        return 0;
    }
    if (node_count <= 0 || node_count > MAX_NODES) {
        return 0;
    }
    
    int edge_count;
    if (scanf("%d", &edge_count) != 1) {
        return 0;
    }
    if (edge_count < 0 || edge_count > node_count * node_count) {
        return 0;
    }
    
    for (int i = 0; i < edge_count; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            return 0;
        }
        if (u < 0 || u >= node_count || v < 0 || v >= node_count) {
            return 0;
        }
        if (w < 0) {
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
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }
}

void print_results() {
    printf("Shortest path distances:\n");
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
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
    initialize_graph();
    
    printf("Enter number of nodes and edges: ");
    if (!read_input()) {
        printf("Invalid input\n");
        return 1;
    }
    
    floyd_warshall();
    print_results();
    
    return 0;
}