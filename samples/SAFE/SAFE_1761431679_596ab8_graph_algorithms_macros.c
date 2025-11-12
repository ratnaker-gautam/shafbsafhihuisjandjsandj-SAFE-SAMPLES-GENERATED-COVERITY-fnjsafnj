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

void read_graph() {
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1 || node_count < 1 || node_count > MAX_NODES) {
        printf("Invalid number of nodes\n");
        exit(1);
    }

    printf("Enter adjacency matrix (%dx%d):\n", node_count, node_count);
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            int weight;
            if (scanf("%d", &weight) != 1) {
                printf("Invalid input\n");
                exit(1);
            }
            if (weight < 0 && weight != -1) {
                printf("Invalid weight\n");
                exit(1);
            }
            graph[i][j] = (weight == -1) ? INF : weight;
            dist[i][j] = graph[i][j];
        }
    }
}

void floyd_warshall() {
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

void find_shortest_path() {
    int src, dest;
    printf("\nEnter source and destination nodes (0-%d): ", node_count-1);
    if (scanf("%d %d", &src, &dest) != 2) {
        printf("Invalid input\n");
        return;
    }
    if (!validate_node(src) || !validate_node(dest)) {
        printf("Invalid node indices\n");
        return;
    }
    if (dist[src][dest] == INF) {
        printf("No path exists between %d and %d\n", src, dest);
    } else {
        printf("Shortest distance from %d to %d: %d\n", src, dest, dist[src][dest]);
    }
}

int main() {
    initialize_graph();
    read_graph();
    floyd_warshall();
    print_shortest_paths();
    
    int choice;
    do {
        printf("\n1. Find shortest path between two nodes\n");
        printf("2. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            break;
        }
        switch (choice) {
            case 1:
                find_shortest_path();
                break;
            case 2:
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 2);
    
    return 0;
}