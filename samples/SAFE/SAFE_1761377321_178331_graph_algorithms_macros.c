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
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            exit(EXIT_FAILURE);
        }
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter integer between %d and %d.\n", min, max);
    }
}

void add_edge(int n) {
    int u = read_int("Enter source node: ", 0, n-1);
    int v = read_int("Enter target node: ", 0, n-1);
    int weight = read_int("Enter edge weight: ", 1, 1000);
    
    if (u != v) {
        graph[u][v] = weight;
        graph[v][u] = weight;
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
        int min_node = -1;
        
        for (int i = 0; i < n; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                min_node = i;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int i = 0; i < n; i++) {
            if (!visited[i] && graph[min_node][i] != INF && dist[min_node] != INF) {
                if (dist[min_node] + graph[min_node][i] < dist[i]) {
                    dist[i] = dist[min_node] + graph[min_node][i];
                }
            }
        }
    }
}

void print_shortest_paths(int start, int n) {
    printf("\nShortest paths from node %d:\n", start);
    for (int i = 0; i < n; i++) {
        if (dist[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: distance %d\n", i, dist[i]);
        }
    }
}

int main() {
    int n = read_int("Enter number of nodes (2-100): ", 2, MAX_NODES);
    initialize_graph(n);
    
    int edges = read_int("Enter number of edges: ", 0, n*(n-1)/2);
    for (int i = 0; i < edges; i++) {
        printf("\nAdding edge %d:\n", i+1);
        add_edge(n);
    }
    
    int start_node = read_int("\nEnter starting node for Dijkstra: ", 0, n-1);
    dijkstra(start_node, n);
    print_shortest_paths(start_node, n);
    
    return 0;
}