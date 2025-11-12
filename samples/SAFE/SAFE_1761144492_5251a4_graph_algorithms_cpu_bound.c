//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

void initialize_graph(Graph *g, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid edge nodes\n");
        exit(1);
    }
    if (weight < 0 || weight > 1000) {
        fprintf(stderr, "Invalid edge weight\n");
        exit(1);
    }
    g->adjacency[from][to] = weight;
    g->adjacency[to][from] = weight;
}

int find_min_distance(int distances[], int visited[], int node_count) {
    int min_distance = INT_MAX;
    int min_index = -1;
    for (int i = 0; i < node_count; i++) {
        if (!visited[i] && distances[i] <= min_distance) {
            min_distance = distances[i];
            min_index = i;
        }
    }
    return min_index;
}

void dijkstra_shortest_path(Graph *g, int source) {
    int distances[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        distances[i] = INT_MAX;
        visited[i] = 0;
    }
    distances[source] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int current = find_min_distance(distances, visited, g->node_count);
        if (current == -1) break;
        visited[current] = 1;
        
        for (int neighbor = 0; neighbor < g->node_count; neighbor++) {
            if (!visited[neighbor] && g->adjacency[current][neighbor] != 0) {
                int new_distance;
                if (distances[current] < INT_MAX - g->adjacency[current][neighbor]) {
                    new_distance = distances[current] + g->adjacency[current][neighbor];
                } else {
                    new_distance = INT_MAX;
                }
                if (new_distance < distances[neighbor]) {
                    distances[neighbor] = new_distance;
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < g->node_count; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

int main() {
    Graph g;
    int node_count = 6;
    
    initialize_graph(&g, node_count);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 2);
    add_edge(&g, 1, 2, 1);
    add_edge(&g, 1, 3, 5);
    add_edge(&g, 2, 3, 8);
    add_edge(&g, 2, 4, 10);
    add_edge(&g, 3, 4, 2);
    add_edge(&g, 3, 5, 6);
    add_edge(&g, 4, 5, 3);
    
    printf("Graph with %d nodes created.\n", node_count);
    printf("Running Dijkstra's algorithm from node 0:\n");
    
    dijkstra_shortest_path(&g, 0);
    
    return 0;
}