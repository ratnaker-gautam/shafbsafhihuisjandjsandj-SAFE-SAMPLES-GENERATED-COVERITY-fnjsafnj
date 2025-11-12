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

int read_graph() {
    if (scanf("%d", &node_count) != 1) return 0;
    if (node_count <= 0 || node_count > MAX_NODES) return 0;
    
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            int weight;
            if (scanf("%d", &weight) != 1) return 0;
            if (weight < 0 && weight != -1) return 0;
            graph[i][j] = (weight == -1) ? INF : weight;
            dist[i][j] = graph[i][j];
        }
    }
    return 1;
}

void floyd_warshall() {
    for (int k = 0; k < node_count; k++) {
        for (int i = 0; i < node_count; i++) {
            if (dist[i][k] == INF) continue;
            for (int j = 0; j < node_count; j++) {
                if (dist[k][j] == INF) continue;
                if (dist[i][j] > dist[i][k] + dist[k][j]) {
                    if (dist[i][k] < INF - dist[k][j]) {
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
    if (!read_graph()) {
        printf("Invalid input\n");
        return 1;
    }
    
    floyd_warshall();
    print_shortest_paths();
    
    return 0;
}