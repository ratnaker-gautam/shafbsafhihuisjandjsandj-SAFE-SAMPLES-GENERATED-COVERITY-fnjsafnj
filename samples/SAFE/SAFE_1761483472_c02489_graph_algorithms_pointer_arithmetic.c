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

void init_graph(struct Graph* graph, int nodes, int edges) {
    if (nodes <= 0 || nodes > MAX_NODES || edges <= 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    
    graph->num_nodes = nodes;
    graph->num_edges = edges;
    graph->edges = malloc(edges * sizeof(struct Edge));
    if (graph->edges == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
}

void free_graph(struct Graph* graph) {
    if (graph->edges != NULL) {
        free(graph->edges);
        graph->edges = NULL;
    }
}

int validate_node(struct Graph* graph, int node) {
    return node >= 0 && node < graph->num_nodes;
}

void add_edge(struct Graph* graph, int idx, int src, int dest, int weight) {
    if (idx < 0 || idx >= graph->num_edges) {
        fprintf(stderr, "Invalid edge index\n");
        return;
    }
    
    if (!validate_node(graph, src) || !validate_node(graph, dest)) {
        fprintf(stderr, "Invalid node index\n");
        return;
    }
    
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        return;
    }
    
    struct Edge* edge_ptr = graph->edges + idx;
    edge_ptr->src = src;
    edge_ptr->dest = dest;
    edge_ptr->weight = weight;
}

void bellman_ford(struct Graph* graph, int start) {
    if (!validate_node(graph, start)) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int distances[MAX_NODES];
    for (int i = 0; i < graph->num_nodes; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    for (int i = 0; i < graph->num_nodes - 1; i++) {
        struct Edge* edge_ptr = graph->edges;
        for (int j = 0; j < graph->num_edges; j++) {
            if (distances[edge_ptr->src] != INT_MAX) {
                int new_dist = distances[edge_ptr->src] + edge_ptr->weight;
                if (new_dist < distances[edge_ptr->dest]) {
                    distances[edge_ptr->dest] = new_dist;
                }
            }
            edge_ptr++;
        }
    }
    
    struct Edge* edge_ptr = graph->edges;
    for (int i = 0; i < graph->num_edges; i++) {
        if (distances[edge_ptr->src] != INT_MAX && 
            distances[edge_ptr->src] + edge_ptr->weight < distances[edge_ptr->dest]) {
            fprintf(stderr, "Graph contains negative cycle\n");
            return;
        }
        edge_ptr++;
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

int main() {
    struct Graph graph;
    init_graph(&graph, 5, 8);
    
    add_edge(&graph, 0, 0, 1, 4);
    add_edge(&graph, 1, 0, 2, 2);
    add_edge(&graph, 2, 1, 2, 3);
    add_edge(&graph, 3, 1, 3, 2);
    add_edge(&graph, 4, 1, 4, 3);
    add_edge(&graph, 5, 2, 1, 1);
    add_edge(&graph, 6, 2, 3, 4);
    add_edge(&graph, 7, 2, 4, 5);
    
    printf("Running Bellman-Ford algorithm on sample graph\n");
    bellman_ford(&graph, 0);
    
    free_graph(&graph);
    return 0;
}