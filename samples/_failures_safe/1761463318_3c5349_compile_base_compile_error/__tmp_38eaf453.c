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
        fprintf(stderr, "Invalid node indices\n");
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

void bellman_ford(struct Graph* graph, int source) {
    if (!validate_node(graph, source)) {
        fprintf(stderr, "Invalid source node\n");
        return;
    }
    
    int* dist = malloc(graph->num_nodes * sizeof(int));
    if (dist == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    
    for (int i = 0; i < graph->num_nodes; i++) {
        *(dist + i) = INT_MAX;
    }
    *(dist + source) = 0;
    
    for (int i = 0; i < graph->num_nodes - 1; i++) {
        for (int j = 0; j < graph->num_edges; j++) {
            struct Edge* edge_ptr = graph->edges + j;
            int u = edge_ptr->src;
            int v = edge_ptr->dest;
            int weight = edge_ptr->weight;
            
            if (*(dist + u) != INT_MAX && *(dist + u) + weight < *(dist + v)) {
                *(dist + v) = *(dist + u) + weight;
            }
        }
    }
    
    for (int j = 0; j < graph->num_edges; j++) {
        struct Edge* edge_ptr = graph->edges + j;
        int u = edge_ptr->src;
        int v = edge_ptr->dest;
        int weight = edge_ptr->weight;
        
        if (*(dist + u) != INT_MAX && *(dist + u) + weight < *(dist + v)) {
            fprintf(stderr, "Graph contains negative weight cycle\n");
            free(dist);
            return;
        }
    }
    
    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (*(dist + i) == INT_MAX) {
            printf("Node %d: INFINITY\n", i);
        } else {
            printf("Node %d: %d\n", i, *(dist + i));
        }
    }
    
    free(dist);
}

int main() {
    struct Graph graph;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges (1-%d): ", MAX_EDGES);
    if (scanf("%d", &edges) != 1 || edges <= 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    init_graph(&graph, nodes, edges);
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            free_graph(&graph);
            return 1;
        }
        add_edge(&graph, i, src, dest, weight);
    }
    
    int source;
    printf("Enter source node for shortest paths: ");
    if (scanf("%d", &source) != 1) {
        fprintf(stderr, "Invalid source node\n");
        free_graph(&graph);
        return 1;
    }
    
    bellman_ford