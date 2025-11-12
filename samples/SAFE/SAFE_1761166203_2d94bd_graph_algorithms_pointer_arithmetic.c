//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    struct Edge edges[MAX_EDGES];
};

void initialize_graph(struct Graph *graph, int nodes) {
    if (graph == NULL || nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid graph initialization parameters\n");
        exit(EXIT_FAILURE);
    }
    graph->num_nodes = nodes;
    graph->num_edges = 0;
}

void add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph == NULL || src < 0 || dest < 0 || src >= graph->num_nodes || dest >= graph->num_nodes) {
        fprintf(stderr, "Invalid edge parameters\n");
        exit(EXIT_FAILURE);
    }
    if (graph->num_edges >= MAX_EDGES) {
        fprintf(stderr, "Graph edge limit exceeded\n");
        exit(EXIT_FAILURE);
    }
    struct Edge *edge_ptr = graph->edges + graph->num_edges;
    edge_ptr->src = src;
    edge_ptr->dest = dest;
    edge_ptr->weight = weight;
    graph->num_edges++;
}

void bellman_ford(struct Graph *graph, int start) {
    if (graph == NULL || start < 0 || start >= graph->num_nodes) {
        fprintf(stderr, "Invalid Bellman-Ford parameters\n");
        exit(EXIT_FAILURE);
    }
    
    int distances[MAX_NODES];
    for (int i = 0; i < graph->num_nodes; i++) {
        distances[i] = 1000000;
    }
    distances[start] = 0;
    
    for (int i = 0; i < graph->num_nodes - 1; i++) {
        for (int j = 0; j < graph->num_edges; j++) {
            struct Edge *edge = graph->edges + j;
            if (distances[edge->src] != 1000000 && distances[edge->src] + edge->weight < distances[edge->dest]) {
                distances[edge->dest] = distances[edge->src] + edge->weight;
            }
        }
    }
    
    for (int j = 0; j < graph->num_edges; j++) {
        struct Edge *edge = graph->edges + j;
        if (distances[edge->src] != 1000000 && distances[edge->src] + edge->weight < distances[edge->dest]) {
            fprintf(stderr, "Graph contains negative weight cycle\n");
            exit(EXIT_FAILURE);
        }
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (distances[i] == 1000000) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

int main() {
    struct Graph graph;
    int num_nodes, num_edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1 || num_nodes < 1 || num_nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return EXIT_FAILURE;
    }
    
    initialize_graph(&graph, num_nodes);
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter edges as 'source destination weight' (0-indexed nodes):\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return EXIT_FAILURE;
        }
        add_edge(&graph, src, dest, weight);
    }
    
    int start_node;
    printf("Enter starting node (0-%d): ", num_nodes - 1);
    if (scanf("%d", &start_node) != 1 || start_node < 0 || start_node >= num_nodes) {
        fprintf(stderr, "Invalid starting node\n");
        return EXIT_FAILURE;
    }
    
    bellman_ford(&graph, start_node);
    
    return EXIT_SUCCESS;
}