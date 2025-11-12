//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: graph_algorithms
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

void dijkstra_shortest_path(Graph *g, int source, int distances[]) {
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[source] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_distance = INT_MAX;
        int min_index = -1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && distances[v] <= min_distance) {
                min_distance = distances[v];
                min_index = v;
            }
        }
        
        if (min_index == -1) break;
        
        visited[min_index] = 1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && g->adjacency[min_index][v] != 0) {
                long new_dist = (long)distances[min_index] + g->adjacency[min_index][v];
                if (new_dist < distances[v] && new_dist <= INT_MAX) {
                    distances[v] = (int)new_dist;
                }
            }
        }
    }
}

int main() {
    Graph graph;
    int node_count = 6;
    
    initialize_graph(&graph, node_count);
    
    add_edge(&graph, 0, 1, 4);
    add_edge(&graph, 0, 2, 2);
    add_edge(&graph, 1, 2, 1);
    add_edge(&graph, 1, 3, 5);
    add_edge(&graph, 2, 3, 8);
    add_edge(&graph, 2, 4, 10);
    add_edge(&graph, 3, 4, 2);
    add_edge(&graph, 3, 5, 6);
    add_edge(&graph, 4, 5, 3);
    
    int source_node;
    printf("Enter source node (0-%d): ", node_count - 1);
    if (scanf("%d", &source_node) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (source_node < 0 || source_node >= node_count) {
        fprintf(stderr, "Invalid source node\n");
        return 1;
    }
    
    int distances[MAX_NODES];
    dijkstra_shortest_path(&graph, source_node, distances);
    
    printf("Shortest distances from node %d:\n", source_node);
    for (int i = 0; i < node_count; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
    
    return 0;
}