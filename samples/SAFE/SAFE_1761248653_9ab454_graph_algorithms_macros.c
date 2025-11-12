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

int read_int(const char* prompt, int min, int max) {
    int value;
    printf("%s", prompt);
    if (scanf("%d", &value) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        exit(EXIT_FAILURE);
    }
    if (value < min || value > max) {
        fprintf(stderr, "Error: Value out of range\n");
        exit(EXIT_FAILURE);
    }
    return value;
}

void add_edge(int n) {
    int u = read_int("Enter source node: ", 0, n-1);
    int v = read_int("Enter destination node: ", 0, n-1);
    int w = read_int("Enter weight: ", 1, 1000);
    
    if (u == v) {
        fprintf(stderr, "Error: Self-loop not allowed\n");
        return;
    }
    graph[u][v] = w;
    graph[v][u] = w;
}

void dijkstra(int n, int start) {
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
                long new_dist = (long)dist[u] + graph[u][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
}

void print_distances(int n, int start) {
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
    int n = read_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    initialize_graph(n);
    
    int edges = read_int("Enter number of edges: ", 0, n*(n-1)/2);
    for (int i = 0; i < edges; i++) {
        add_edge(n);
    }
    
    int start = read_int("Enter starting node for Dijkstra: ", 0, n-1);
    dijkstra(n, start);
    print_distances(n, start);
    
    return 0;
}