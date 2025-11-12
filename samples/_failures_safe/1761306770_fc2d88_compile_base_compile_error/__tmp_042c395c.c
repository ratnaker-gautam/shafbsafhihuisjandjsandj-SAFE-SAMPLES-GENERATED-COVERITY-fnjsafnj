//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 500

struct Graph {
    int num_nodes;
    int num_edges;
    int adj_matrix[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph *g) {
    if (g == NULL) return;
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            *(*(g->adj_matrix + i) + j) = 0;
        }
    }
}

int add_edge(struct Graph *g, int u, int v, int weight) {
    if (g == NULL) return 0;
    if (u < 0 || u >= MAX_NODES || v < 0 || v >= MAX_NODES) return 0;
    if (u == v) return 0;
    if (weight <= 0) return 0;
    
    if (*(*(g->adj_matrix + u) + v) == 0) {
        g->num_edges++;
    }
    *(*(g->adj_matrix + u) + v) = weight;
    *(*(g->adj_matrix + v) + u) = weight;
    return 1;
}

void dijkstra(struct Graph *g, int source, int *dist) {
    if (g == NULL || dist == NULL) return;
    if (source < 0 || source >= g->num_nodes) return;
    
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->num_nodes; i++) {
        *(dist + i) = INT_MAX;
    }
    *(dist + source) = 0;
    
    for (int count = 0; count < g->num_nodes; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!*(visited + v) && *(dist + v) <= min_dist) {
                min_dist = *(dist + v);
                min_node = v;
            }
        }
        
        if (min_node == -1) break;
        *(visited + min_node) = 1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            int edge_weight = *(*(g->adj_matrix + min_node) + v);
            if (edge_weight > 0 && !*(visited + v)) {
                int new_dist = *(dist + min_node) + edge_weight;
                if (*(dist + min_node) != INT_MAX && new_dist < *(dist + v)) {
                    *(dist + v) = new_dist;
                }
            }
        }
    }
}

int read_int(const char *prompt, int min, int max) {
    char buffer[100];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char *endptr;
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d.\n", min, max);
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Graph Algorithms with Dijkstra\n");
    printf("==============================\n\n");
    
    graph.num_nodes = read_int("Enter number of nodes (2-100): ", 2, MAX_NODES);
    if (graph.num_nodes == -1) return 1;
    
    int num_edges = read_int("Enter number of edges: ", 1, MAX_EDGES);
    if (num_edges == -1) return 1;
    
    printf("\nEnter edges (u v weight):\n");
    for (int i = 0; i < num_edges; i++) {
        printf("Edge %d: ", i + 1);
        
        int u = read_int("", 0, graph.num_nodes - 1);
        if (u == -1) return 1;
        
        int v = read_int("", 0, graph.num_nodes - 1);
        if (v == -1) return 1;
        
        int weight = read_int("", 1, 1000);
        if (weight == -1) return 1;
        
        if (!add_edge(&graph, u, v, weight)) {
            printf("Failed to add edge. Please try again.\n");
            i--;
        }
    }
    
    int source = read_int("\nEnter source node for Dijkstra: ", 0, graph.num_nodes - 1);
    if (source == -1) return 1;
    
    int distances[MAX_NODES];
    dijkstra(&graph, source, distances);
    
    printf("\nShortest distances from node %d:\n", source);
    for (int i = 0; i < graph.num_nodes; i++) {
        if (*(distances + i) ==