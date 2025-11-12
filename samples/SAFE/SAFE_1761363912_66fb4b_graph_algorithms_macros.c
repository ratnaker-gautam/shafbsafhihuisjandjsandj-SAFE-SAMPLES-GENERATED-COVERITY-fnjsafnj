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

    printf("Enter adjacency matrix (%dx%d):\n", node_count, node_count);
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            int weight;
            if (scanf("%d", &weight) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (weight < 0 && weight != -1) {
                printf("Invalid weight\n");
                return 1;
            }
            graph[i][j] = (weight == -1) ? INF : weight;
        }
    }

    floyd_warshall();
    print_shortest_paths();

    printf("\nEnter source and destination nodes to query (0-%d): ", node_count-1);
    int src, dest;
    if (scanf("%d %d", &src, &dest) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_node(src) || !validate_node(dest)) {
        printf("Invalid nodes\n");
        return 1;
    }

    if (dist[src][dest] == INF) {
        printf("No path from %d to %d\n", src, dest);
    } else {
        printf("Shortest distance from %d to %d: %d\n", src, dest, dist[src][dest]);
    }

    return 0;
}