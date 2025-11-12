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

void init_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
}

int add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g->num_edges >= MAX_EDGES) return 0;
    if (src < 0 || dest < 0 || src >= MAX_NODES || dest >= MAX_NODES) return 0;
    
    struct Edge *e = g->edges + g->num_edges;
    e->src = src;
    e->dest = dest;
    e->weight = weight;
    g->num_edges++;
    
    if (src >= g->num_nodes) g->num_nodes = src + 1;
    if (dest >= g->num_nodes) g->num_nodes = dest + 1;
    
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
        printf("Invalid start node\n");
        return;
    }
    
    int dist[MAX_NODES];
    for (int i = 0; i < MAX_NODES; i++) {
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
    init_graph(&g);
    
    printf("Adding edges to graph...\n");
    
    if (!add_edge(&g, 0, 1, 4)) return 1;
    if (!add_edge(&g, 0, 2, 2)) return 1;
    if (!add_edge(&g, 1, 2, 1)) return 1;
    if (!add_edge(&g, 1, 3, 5)) return 1;
    if (!add_edge(&g, 2, 3, 8)) return 1;
    if (!add_edge(&g, 2, 4, 10)) return 1;
    if (!add_edge(&g, 3, 4, 2)) return 1;
    if (!add_edge(&g, 4, 3, 1)) return 1;
    
    print_graph(&g);
    printf("\n");
    
    printf("Running Bellman-Ford algorithm from node 0:\n");
    bellman_ford(&g, 0);
    
    printf("\nRunning Bellman-Ford algorithm from node 3:\n");
    bellman_ford(&g, 3);
    
    return 0;
}