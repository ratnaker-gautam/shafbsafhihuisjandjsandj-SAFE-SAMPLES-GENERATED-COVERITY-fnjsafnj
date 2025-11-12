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

void init_graph(struct Graph *g, int nodes) {
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid graph initialization\n");
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = 0;
}

int add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g == NULL || src < 0 || src >= g->num_nodes || 
        dest < 0 || dest >= g->num_nodes || g->num_edges >= MAX_EDGES) {
        return 0;
    }
    
    struct Edge *edge_ptr = g->edges + g->num_edges;
    edge_ptr->src = src;
    edge_ptr->dest = dest;
    edge_ptr->weight = weight;
    g->num_edges++;
    return 1;
}

void print_graph(struct Graph *g) {
    if (g == NULL) return;
    
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    struct Edge *edge_ptr = g->edges;
    struct Edge *end_ptr = g->edges + g->num_edges;
    
    while (edge_ptr < end_ptr) {
        printf("  %d -> %d (weight: %d)\n", edge_ptr->src, edge_ptr->dest, edge_ptr->weight);
        edge_ptr++;
    }
}

void bellman_ford(struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->num_nodes) {
        fprintf(stderr, "Invalid parameters for Bellman-Ford\n");
        return;
    }
    
    int dist[MAX_NODES];
    int i;
    
    int *dist_ptr = dist;
    int *dist_end = dist + g->num_nodes;
    while (dist_ptr < dist_end) {
        *dist_ptr = 999999;
        dist_ptr++;
    }
    dist[start] = 0;
    
    for (i = 0; i < g->num_nodes - 1; i++) {
        struct Edge *edge_ptr = g->edges;
        struct Edge *edge_end = g->edges + g->num_edges;
        
        while (edge_ptr < edge_end) {
            if (dist[edge_ptr->src] != 999999 && 
                dist[edge_ptr->src] + edge_ptr->weight < dist[edge_ptr->dest]) {
                dist[edge_ptr->dest] = dist[edge_ptr->src] + edge_ptr->weight;
            }
            edge_ptr++;
        }
    }
    
    struct Edge *edge_ptr = g->edges;
    struct Edge *edge_end = g->edges + g->num_edges;
    while (edge_ptr < edge_end) {
        if (dist[edge_ptr->src] != 999999 && 
            dist[edge_ptr->src] + edge_ptr->weight < dist[edge_ptr->dest]) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
        edge_ptr++;
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (i = 0; i < g->num_nodes; i++) {
        if (dist[i] == 999999) {
            printf("  Node %d: unreachable\n", i);
        } else {
            printf("  Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph graph;
    int num_nodes = 5;
    
    init_graph(&graph, num_nodes);
    
    add_edge(&graph, 0, 1, 4);
    add_edge(&graph, 0, 2, 2);
    add_edge(&graph, 1, 2, 3);
    add_edge(&graph, 1, 3, 2);
    add_edge(&graph, 1, 4, 3);
    add_edge(&graph, 2, 1, 1);
    add_edge(&graph, 2, 3, 4);
    add_edge(&graph, 3, 4, 2);
    add_edge(&graph, 4, 0, 1);
    
    print_graph(&graph);
    printf("\n");
    
    bellman_ford(&graph, 0);
    
    return 0;
}