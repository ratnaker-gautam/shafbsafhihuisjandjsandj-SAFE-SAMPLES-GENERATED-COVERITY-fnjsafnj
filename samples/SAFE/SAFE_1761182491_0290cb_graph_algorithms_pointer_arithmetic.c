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

void init_graph(struct Graph *g, int nodes, int edges) {
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = edges;
    memset(g->edges, 0, sizeof(g->edges));
}

int add_edge(struct Graph *g, int src, int dest, int weight, int idx) {
    if (g == NULL || idx < 0 || idx >= MAX_EDGES) return 0;
    if (src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes) return 0;
    
    struct Edge *e = g->edges + idx;
    e->src = src;
    e->dest = dest;
    e->weight = weight;
    return 1;
}

void print_graph(struct Graph *g) {
    if (g == NULL) return;
    
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    for (int i = 0; i < g->num_edges; i++) {
        struct Edge *e = g->edges + i;
        printf("Edge %d: %d -> %d (weight %d)\n", i, e->src, e->dest, e->weight);
    }
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
            struct Edge *e = g->edges + j;
            if (dist[e->src] != 1000000 && dist[e->src] + e->weight < dist[e->dest]) {
                dist[e->dest] = dist[e->src] + e->weight;
            }
        }
    }
    
    for (int j = 0; j < g->num_edges; j++) {
        struct Edge *e = g->edges + j;
        if (dist[e->src] != 1000000 && dist[e->src] + e->weight < dist[e->dest]) {
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
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    init_graph(&g, nodes, edges);
    
    printf("Enter edges as: source destination weight\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        printf("Edge %d: ", i);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&g, src, dest, weight, i)) {
            fprintf(stderr, "Failed to add edge\n");
            return 1;
        }
    }
    
    print_graph(&g);
    
    int start_node;
    printf("Enter starting node for Bellman-Ford: ");
    if (scanf("%d", &start_node) != 1 || start_node < 0 || start_node >= nodes) {
        fprintf(stderr, "Invalid starting node\n");
        return 1;
    }
    
    bellman_ford(&g, start_node);
    
    return 0;
}