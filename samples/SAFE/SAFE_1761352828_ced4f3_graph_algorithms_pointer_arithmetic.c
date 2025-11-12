//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Edge {
    int src;
    int dest;
    int weight;
};

struct Graph {
    int num_nodes;
    int num_edges;
    struct Edge* edges;
};

int validate_graph_inputs(int nodes, int edges) {
    if (nodes <= 0 || nodes > MAX_NODES) return 0;
    if (edges < 0 || edges > MAX_EDGES) return 0;
    if (edges > nodes * (nodes - 1)) return 0;
    return 1;
}

int validate_edge(int src, int dest, int weight, int num_nodes) {
    if (src < 0 || src >= num_nodes) return 0;
    if (dest < 0 || dest >= num_nodes) return 0;
    if (src == dest) return 0;
    if (weight < 0 || weight > 10000) return 0;
    return 1;
}

void initialize_graph(struct Graph* graph, int nodes, int edges) {
    graph->num_nodes = nodes;
    graph->num_edges = edges;
    graph->edges = malloc(edges * sizeof(struct Edge));
    if (graph->edges == NULL) {
        exit(EXIT_FAILURE);
    }
}

void bellman_ford(struct Graph* graph, int source, int* dist) {
    int num_nodes = graph->num_nodes;
    int num_edges = graph->num_edges;
    
    for (int i = 0; i < num_nodes; i++) {
        *(dist + i) = INT_MAX;
    }
    *(dist + source) = 0;
    
    for (int i = 1; i < num_nodes; i++) {
        for (int j = 0; j < num_edges; j++) {
            struct Edge* edge = graph->edges + j;
            int u = edge->src;
            int v = edge->dest;
            int weight = edge->weight;
            
            if (*(dist + u) != INT_MAX && *(dist + u) + weight < *(dist + v)) {
                *(dist + v) = *(dist + u) + weight;
            }
        }
    }
    
    for (int j = 0; j < num_edges; j++) {
        struct Edge* edge = graph->edges + j;
        int u = edge->src;
        int v = edge->dest;
        int weight = edge->weight;
        
        if (*(dist + u) != INT_MAX && *(dist + u) + weight < *(dist + v)) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }
}

void print_distances(int* dist, int num_nodes, int source) {
    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < num_nodes; i++) {
        if (*(dist + i) == INT_MAX) {
            printf("Node %d: INFINITY\n", i);
        } else {
            printf("Node %d: %d\n", i, *(dist + i));
        }
    }
}

int main() {
    struct Graph graph;
    int num_nodes, num_edges;
    int source_node;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &num_edges) != 1) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (!validate_graph_inputs(num_nodes, num_edges)) {
        printf("Invalid graph parameters\n");
        return EXIT_FAILURE;
    }
    
    initialize_graph(&graph, num_nodes, num_edges);
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            free(graph.edges);
            return EXIT_FAILURE;
        }
        
        if (!validate_edge(src, dest, weight, num_nodes)) {
            printf("Invalid edge parameters\n");
            free(graph.edges);
            return EXIT_FAILURE;
        }
        
        (graph.edges + i)->src = src;
        (graph.edges + i)->dest = dest;
        (graph.edges + i)->weight = weight;
    }
    
    printf("Enter source node (0-%d): ", num_nodes - 1);
    if (scanf("%d", &source_node) != 1) {
        printf("Invalid source node\n");
        free(graph.edges);
        return EXIT_FAILURE;
    }
    
    if (source_node < 0 || source_node >= num_nodes) {
        printf("Invalid source node\n");
        free(graph.edges);
        return EXIT_FAILURE;
    }
    
    int distances[MAX_NODES];
    bellman_ford(&graph, source_node, distances);
    print_distances(distances, num_nodes, source_node);
    
    free(graph.edges);
    return EXIT_SUCCESS;
}