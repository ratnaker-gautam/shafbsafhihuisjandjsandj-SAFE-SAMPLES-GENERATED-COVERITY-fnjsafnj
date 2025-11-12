//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
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

void bellman_ford(struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->num_nodes) {
        fprintf(stderr, "Invalid parameters for Bellman-Ford\n");
        return;
    }
    
    int dist[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = 1000000;
    }
    dist[start] = 0;
    
    for (int i = 0; i < g->num_nodes - 1; i++) {
        for (int j = 0; j < g->num_edges; j++) {
            struct Edge *edge = g->edges + j;
            if (dist[edge->src] != 1000000 && 
                dist[edge->src] + edge->weight < dist[edge->dest]) {
                dist[edge->dest] = dist[edge->src] + edge->weight;
            }
        }
    }
    
    for (int j = 0; j < g->num_edges; j++) {
        struct Edge *edge = g->edges + j;
        if (dist[edge->src] != 1000000 && 
            dist[edge->src] + edge->weight < dist[edge->dest]) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < g->num_nodes; i++) {
        printf("Node %d: %d\n", i, dist[i]);
    }
}

int main() {
    struct Graph graph;
    int num_nodes, num_edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &num_nodes) != 1 || num_nodes < 1 || num_nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    
    init_graph(&graph, num_nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        printf("Enter edge %d (src dest weight): ", i + 1);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&graph, src, dest, weight)) {
            fprintf(stderr, "Failed to add edge\n");
            return 1;
        }
    }
    
    int start_node;
    printf("Enter starting node: ");
    if (scanf("%d", &start_node) != 1 || start_node < 0 || start_node >= num_nodes) {
        fprintf(stderr, "Invalid starting node\n");
        return 1;
    }
    
    bellman_ford(&graph, start_node);
    
    return 0;
}