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

int get_valid_int(int min_val, int max_val) {
    int value;
    while (1) {
        if (scanf("%d", &value) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Enter a number between %d and %d: ", min_val, max_val);
            continue;
        }
        if (value >= min_val && value <= max_val) {
            return value;
        }
        printf("Value out of range. Enter a number between %d and %d: ", min_val, max_val);
    }
}

void dijkstra(int start, int n) {
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[start] = 0;

    for (int count = 0; count < n; count++) {
        int min_dist = INF;
        int u = -1;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                u = i;
            }
        }
        if (u == -1 || dist[u] == INF) break;
        visited[u] = 1;

        for (int v = 0; v < n; v++) {
            if (!visited[v] && graph[u][v] != INF && dist[u] != INF) {
                long long new_dist = (long long)dist[u] + graph[u][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
}

int main() {
    int n, edges, start;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    n = get_valid_int(1, MAX_NODES);
    
    printf("Enter number of edges: ");
    edges = get_valid_int(0, n * (n - 1));

    initialize_graph(n);

    printf("Enter edges (from to weight):\n");
    for (int i = 0; i < edges; i++) {
        int from, to, weight;
        printf("Edge %d: ", i + 1);
        from = get_valid_int(0, n - 1);
        to = get_valid_int(0, n - 1);
        weight = get_valid_int(1, 10000);
        
        if (from != to) {
            graph[from][to] = weight;
        }
    }

    printf("Enter starting node (0-%d): ", n - 1);
    start = get_valid_int(0, n - 1);

    dijkstra(start, n);

    printf("\nShortest distances from node %d:\n", start);
    for (int i = 0; i < n; i++) {
        if (dist[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }

    return 0;
}