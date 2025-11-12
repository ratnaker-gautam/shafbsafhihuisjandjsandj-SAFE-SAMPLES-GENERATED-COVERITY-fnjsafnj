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
    if (src < 0 || src >= MAX_NODES) return 0;
    if (dest < 0 || dest >= MAX_NODES) return 0;
    if (weight < 0) return 0;
    
    struct Edge *e = g->edges + g->num_edges;
    e->src = src;
    e->dest = dest;
    e->weight = weight;
    g->num_edges++;
    if (src >= g->num_nodes) g->num_nodes = src + 1;
    if (dest >= g->num_nodes) g->num_nodes = dest + 1;
    return 1;
}

void bellman_ford(struct Graph *g, int start, int dist[]) {
    if (start < 0 || start >= g->num_nodes) return;
    
    for (int i = 0; i < g->num_nodes; i++) {
        *(dist + i) = 1000000;
    }
    *(dist + start) = 0;
    
    for (int i = 0; i < g->num_nodes - 1; i++) {
        for (int j = 0; j < g->num_edges; j++) {
            struct Edge *e = g->edges + j;
            if (*(dist + e->src) != 1000000 && *(dist + e->src) + e->weight < *(dist + e->dest)) {
                *(dist + e->dest) = *(dist + e->src) + e->weight;
            }
        }
    }
    
    for (int j = 0; j < g->num_edges; j++) {
        struct Edge *e = g->edges + j;
        if (*(dist + e->src) != 1000000 && *(dist + e->src) + e->weight < *(dist + e->dest)) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }
}

void print_distances(struct Graph *g, int dist[]) {
    printf("Shortest distances from node 0:\n");
    for (int i = 0; i < g->num_nodes; i++) {
        if (*(dist + i) == 1000000) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, *(dist + i));
        }
    }
}

int main() {
    struct Graph g;
    init_graph(&g);
    
    int edges[][3] = {
        {0, 1, 4},
        {0, 2, 1},
        {2, 1, 2},
        {1, 3, 1},
        {2, 3, 5},
        {3, 4, 3}
    };
    
    int num_edges = sizeof(edges) / sizeof(edges[0]);
    
    for (int i = 0; i < num_edges; i++) {
        int *edge = *(edges + i);
        if (!add_edge(&g, *edge, *(edge + 1), *(edge + 2))) {
            printf("Failed to add edge %d\n", i);
            return 1;
        }
    }
    
    int dist[MAX_NODES];
    bellman_ford(&g, 0, dist);
    print_distances(&g, dist);
    
    return 0;
}