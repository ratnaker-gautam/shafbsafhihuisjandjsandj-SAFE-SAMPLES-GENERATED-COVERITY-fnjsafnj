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
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = 0;
}

int add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g->num_edges >= MAX_EDGES) return 0;
    if (src < 0 || src >= g->num_nodes) return 0;
    if (dest < 0 || dest >= g->num_nodes) return 0;
    
    struct Edge *e = g->edges + g->num_edges;
    e->src = src;
    e->dest = dest;
    e->weight = weight;
    g->num_edges++;
    return 1;
}

void print_graph(struct Graph *g) {
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    struct Edge *end = g->edges + g->num_edges;
    for (struct Edge *e = g->edges; e != end; e++) {
        printf("  %d -> %d (weight: %d)\n", e->src, e->dest, e->weight);
    }
}

void bellman_ford(struct Graph *g, int start) {
    if (start < 0 || start >= g->num_nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int dist[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = 1000000;
    }
    dist[start] = 0;
    
    for (int i = 0; i < g->num_nodes - 1; i++) {
        struct Edge *end = g->edges + g->num_edges;
        for (struct Edge *e = g->edges; e != end; e++) {
            if (dist[e->src] != 1000000 && dist[e->src] + e->weight < dist[e->dest]) {
                dist[e->dest] = dist[e->src] + e->weight;
            }
        }
    }
    
    struct Edge *end = g->edges + g->num_edges;
    for (struct Edge *e = g->edges; e != end; e++) {
        if (dist[e->src] != 1000000 && dist[e->src] + e->weight < dist[e->dest]) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < g->num_nodes; i++) {
        if (dist[i] == 1000000) {
            printf("  Node %d: unreachable\n", i);
        } else {
            printf("  Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph g;
    init_graph(&g, 5);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 2);
    add_edge(&g, 1, 2, 3);
    add_edge(&g, 1, 3, 2);
    add_edge(&g, 1, 4, 3);
    add_edge(&g, 2, 1, 1);
    add_edge(&g, 2, 3, 4);
    add_edge(&g, 2, 4, 5);
    add_edge(&g, 3, 4, 1);
    
    print_graph(&g);
    printf("\n");
    
    bellman_ford(&g, 0);
    
    return 0;
}