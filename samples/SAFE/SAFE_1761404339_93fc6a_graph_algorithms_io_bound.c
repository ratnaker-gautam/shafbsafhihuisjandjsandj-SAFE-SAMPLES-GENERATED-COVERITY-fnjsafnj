//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 500

struct Graph {
    int nodes;
    int edges[MAX_EDGES][3];
    int edge_count;
};

void initialize_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        exit(1);
    }
    g->nodes = nodes;
    g->edge_count = 0;
}

void add_edge(struct Graph *g, int u, int v, int weight) {
    if (g->edge_count >= MAX_EDGES) {
        printf("Too many edges\n");
        return;
    }
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        printf("Invalid node indices\n");
        return;
    }
    g->edges[g->edge_count][0] = u;
    g->edges[g->edge_count][1] = v;
    g->edges[g->edge_count][2] = weight;
    g->edge_count++;
}

void bellman_ford(struct Graph *g, int source) {
    if (source < 0 || source >= g->nodes) {
        printf("Invalid source node\n");
        return;
    }
    
    int dist[MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INT_MAX;
    }
    dist[source] = 0;
    
    for (int i = 0; i < g->nodes - 1; i++) {
        for (int j = 0; j < g->edge_count; j++) {
            int u = g->edges[j][0];
            int v = g->edges[j][1];
            int weight = g->edges[j][2];
            
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
            }
        }
    }
    
    for (int j = 0; j < g->edge_count; j++) {
        int u = g->edges[j][0];
        int v = g->edges[j][1];
        int weight = g->edges[j][2];
        
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }
    
    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < g->nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    initialize_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (edges < 0 || edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'u v weight' (nodes are 0-indexed):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        add_edge(&g, u, v, weight);
    }
    
    int source;
    printf("Enter source node: ");
    if (scanf("%d", &source) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    bellman_ford(&g, source);
    
    return 0;
}