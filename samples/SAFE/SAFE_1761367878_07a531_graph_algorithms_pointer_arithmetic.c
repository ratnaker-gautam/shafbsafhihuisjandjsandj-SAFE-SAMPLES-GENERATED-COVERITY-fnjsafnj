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
    return 1;
}

void bellman_ford(struct Graph *g, int start) {
    if (g->num_nodes == 0) return;
    if (start < 0 || start >= g->num_nodes) return;
    
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
        if (dist[i] == 1000000) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph g;
    init_graph(&g);
    
    printf("Enter number of nodes (max %d): ", MAX_NODES);
    int num_nodes;
    if (scanf("%d", &num_nodes) != 1 || num_nodes <= 0 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    g.num_nodes = num_nodes;
    
    printf("Enter number of edges (max %d): ", MAX_EDGES);
    int num_edges;
    if (scanf("%d", &num_edges) != 1 || num_edges <= 0 || num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'source destination weight':\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge format\n");
            return 1;
        }
        if (src < 0 || dest < 0 || src >= num_nodes || dest >= num_nodes) {
            printf("Invalid node index\n");
            return 1;
        }
        if (!add_edge(&g, src, dest, weight)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    printf("Enter starting node: ");
    int start;
    if (scanf("%d", &start) != 1 || start < 0 || start >= num_nodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    
    bellman_ford(&g, start);
    
    return 0;
}