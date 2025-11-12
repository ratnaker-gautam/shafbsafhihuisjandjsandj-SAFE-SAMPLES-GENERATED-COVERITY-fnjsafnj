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
        fprintf(stderr, "Error: Invalid input\n");
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
        fprintf(stderr, "Error: Invalid node index\n");
        exit(EXIT_FAILURE);
    }
    if (weight < 0) {
        fprintf(stderr, "Error: Negative weights not allowed\n");
        exit(EXIT_FAILURE);
    }
    graph[u][v] = weight;
    graph[v][u] = weight;
}

int find_min_distance(int n) {
    int min_dist = INF;
    int min_index = -1;
    for (int i = 0; i < n; i++) {
        if (!visited[i] && dist[i] < min_dist) {
            min_dist = dist[i];
            min_index = i;
        }
    }
    return min_index;
}

void dijkstra(int start, int n) {
    if (start < 0 || start >= n) {
        fprintf(stderr, "Error: Invalid start node\n");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[start] = 0;
    
    for (int count = 0; count < n; count++) {
        int u = find_min_distance(n);
        if (u == -1) break;
        
        visited[u] = 1;
        
        for (int v = 0; v < n; v++) {
            if (!visited[v] && graph[u][v] != INF && dist[u] != INF) {
                long new_dist = (long)dist[u] + graph[u][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
}

void print_distances(int start, int n) {
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < n; i++) {
        if (dist[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    int n, m, start;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    n = read_int();
    if (n < 1 || n > MAX_NODES) {
        fprintf(stderr, "Error: Invalid number of nodes\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter number of edges: ");
    m = read_int();
    if (m < 0 || m > n * (n - 1) / 2) {
        fprintf(stderr, "Error: Invalid number of edges\n");
        return EXIT_FAILURE;
    }
    
    initialize_graph(n);
    
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < m; i++) {
        int u = read_int();
        int v = read_int();
        int weight = read_int();
        add_edge(u, v, weight, n);
    }
    
    printf("Enter start node: ");
    start = read_int();
    
    dijkstra(start, n);
    print_distances(start, n);
    
    return EXIT_SUCCESS;
}