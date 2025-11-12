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

void bellman_ford(struct Graph *g, int start, int *dist) {
    for (int i = 0; i < g->num_nodes; i++) {
        *(dist + i) = 999999;
    }
    *(dist + start) = 0;
    
    for (int i = 0; i < g->num_nodes - 1; i++) {
        for (int j = 0; j < g->num_edges; j++) {
            struct Edge *e = g->edges + j;
            if (*(dist + e->src) != 999999 && *(dist + e->src) + e->weight < *(dist + e->dest)) {
                *(dist + e->dest) = *(dist + e->src) + e->weight;
            }
        }
    }
    
    for (int j = 0; j < g->num_edges; j++) {
        struct Edge *e = g->edges + j;
        if (*(dist + e->src) != 999999 && *(dist + e->src) + e->weight < *(dist + e->dest)) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }
}

void print_distances(int *dist, int num_nodes, int start) {
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < num_nodes; i++) {
        printf("To node %d: ", i);
        if (*(dist + i) == 999999) {
            printf("Unreachable\n");
        } else {
            printf("%d\n", *(dist + i));
        }
    }
}

int main() {
    struct Graph g;
    init_graph(&g);
    
    int edges[][3] = {
        {0, 1, 4},
        {0, 2, 2},
        {1, 2, 3},
        {1, 3, 2},
        {1, 4, 3},
        {2, 1, 1},
        {2, 3, 4},
        {2, 4, 5},
        {4, 3, 1}
    };
    
    int num_edges = sizeof(edges) / sizeof(edges[0]);
    
    for (int i = 0; i < num_edges; i++) {
        if (!add_edge(&g, edges[i][0], edges[i][1], edges[i][2])) {
            printf("Failed to add edge %d\n", i);
            return 1;
        }
    }
    
    int dist[MAX_NODES];
    int start_node = 0;
    
    bellman_ford(&g, start_node, dist);
    print_distances(dist, g.num_nodes, start_node);
    
    return 0;
}