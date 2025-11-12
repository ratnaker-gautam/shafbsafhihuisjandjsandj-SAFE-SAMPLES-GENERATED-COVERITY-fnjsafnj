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
    struct Edge edges[MAX_EDGES];
};

void init_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
}

int add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g->num_edges >= MAX_EDGES) return 0;
    if (src < 0 || dest < 0) return 0;
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

int bellman_ford(struct Graph *g, int src, int dist[]) {
    if (src < 0 || src >= g->num_nodes) return 0;
    
    for (int i = 0; i < g->num_nodes; i++) {
        *(dist + i) = INT_MAX;
    }
    *(dist + src) = 0;
    
    for (int i = 1; i < g->num_nodes; i++) {
        for (int j = 0; j < g->num_edges; j++) {
            struct Edge *e = g->edges + j;
            if (*(dist + e->src) != INT_MAX) {
                int new_dist = *(dist + e->src) + e->weight;
                if (new_dist < *(dist + e->dest)) {
                    *(dist + e->dest) = new_dist;
                }
            }
        }
    }
    
    for (int j = 0; j < g->num_edges; j++) {
        struct Edge *e = g->edges + j;
        if (*(dist + e->src) != INT_MAX) {
            int new_dist = *(dist + e->src) + e->weight;
            if (new_dist < *(dist + e->dest)) {
                return 0;
            }
        }
    }
    
    return 1;
}

void print_distances(int dist[], int n, int src) {
    printf("Shortest distances from node %d:\n", src);
    for (int i = 0; i < n; i++) {
        if (*(dist + i) == INT_MAX) {
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
        {0, 1, 4}, {0, 2, 2}, {1, 2, 3}, {1, 3, 2},
        {1, 4, 3}, {2, 1, 1}, {2, 3, 4}, {2, 4, 5},
        {3, 4, 1}, {4, 3, 2}
    };
    
    int num_edges = sizeof(edges) / sizeof(edges[0]);
    
    for (int i = 0; i < num_edges; i++) {
        if (!add_edge(&g, *(*(edges + i) + 0), *(*(edges + i) + 1), *(*(edges + i) + 2))) {
            printf("Failed to add edge %d\n", i);
            return 1;
        }
    }
    
    int dist[MAX_NODES];
    int src_node = 0;
    
    if (!bellman_ford(&g, src_node, dist)) {
        printf("Graph contains negative weight cycle\n");
        return 1;
    }
    
    print_distances(dist, g.num_nodes, src_node);
    
    printf("\nTesting with user input:\n");
    printf("Enter source node (0-%d): ", g.num_nodes - 1);
    int user_src;
    if (scanf("%d", &user_src) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (user_src < 0 || user_src >= g.num_nodes) {
        printf("Invalid source node\n");
        return 1;
    }
    
    if (!bellman_ford(&g, user_src, dist)) {
        printf("Graph contains negative weight cycle\n");
        return 1;
    }
    
    print_distances(dist, g.num_nodes, user_src);
    
    return 0;
}