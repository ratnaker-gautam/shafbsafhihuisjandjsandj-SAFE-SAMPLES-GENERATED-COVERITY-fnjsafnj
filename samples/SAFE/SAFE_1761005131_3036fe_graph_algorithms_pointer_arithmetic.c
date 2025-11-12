//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
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
    struct Edge *edges;
};

int validate_graph(const struct Graph *g) {
    if (g == NULL) return 0;
    if (g->num_nodes <= 0 || g->num_nodes > MAX_NODES) return 0;
    if (g->num_edges < 0 || g->num_edges > MAX_EDGES) return 0;
    if (g->edges == NULL) return 0;
    for (int i = 0; i < g->num_edges; i++) {
        struct Edge *e = g->edges + i;
        if (e->src < 0 || e->src >= g->num_nodes) return 0;
        if (e->dest < 0 || e->dest >= g->num_nodes) return 0;
        if (e->weight < 0) return 0;
    }
    return 1;
}

void dijkstra(const struct Graph *g, int src, int *dist) {
    if (!validate_graph(g) || src < 0 || src >= g->num_nodes || dist == NULL) return;
    
    int visited[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        *(dist + i) = INT_MAX;
        *(visited + i) = 0;
    }
    *(dist + src) = 0;
    
    for (int count = 0; count < g->num_nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!*(visited + v) && *(dist + v) <= min_dist) {
                min_dist = *(dist + v);
                min_node = v;
            }
        }
        
        if (min_node == -1) break;
        *(visited + min_node) = 1;
        
        for (int i = 0; i < g->num_edges; i++) {
            struct Edge *e = g->edges + i;
            if (e->src == min_node) {
                int alt;
                if (*(dist + min_node) < INT_MAX - e->weight) {
                    alt = *(dist + min_node) + e->weight;
                } else {
                    alt = INT_MAX;
                }
                if (alt < *(dist + e->dest)) {
                    *(dist + e->dest) = alt;
                }
            }
        }
    }
}

int main() {
    struct Graph graph;
    graph.num_nodes = 5;
    graph.num_edges = 7;
    
    struct Edge edges[] = {
        {0, 1, 4}, {0, 2, 1}, {1, 2, 2}, {1, 3, 5},
        {2, 1, 2}, {2, 3, 8}, {2, 4, 10}
    };
    
    graph.edges = edges;
    
    if (!validate_graph(&graph)) {
        printf("Invalid graph\n");
        return 1;
    }
    
    int distances[MAX_NODES];
    dijkstra(&graph, 0, distances);
    
    printf("Shortest distances from node 0:\n");
    for (int i = 0; i < graph.num_nodes; i++) {
        if (*(distances + i) == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, *(distances + i));
        }
    }
    
    return 0;
}