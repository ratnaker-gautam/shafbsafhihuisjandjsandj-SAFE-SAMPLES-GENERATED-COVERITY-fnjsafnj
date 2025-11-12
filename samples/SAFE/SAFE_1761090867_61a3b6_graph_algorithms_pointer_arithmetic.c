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

void print_graph(struct Graph *g) {
    if (g == NULL) return;
    
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    struct Edge *edge_ptr = g->edges;
    struct Edge *end_ptr = g->edges + g->num_edges;
    
    while (edge_ptr < end_ptr) {
        printf("  %d -> %d (weight: %d)\n", edge_ptr->src, edge_ptr->dest, edge_ptr->weight);
        edge_ptr++;
    }
}

void bellman_ford(struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->num_nodes) {
        fprintf(stderr, "Invalid parameters for Bellman-Ford\n");
        return;
    }
    
    int dist[MAX_NODES];
    int i;
    
    for (i = 0; i < g->num_nodes; i++) {
        dist[i] = 1000000;
    }
    dist[start] = 0;
    
    struct Edge *edge_ptr;
    struct Edge *end_ptr;
    int u, v, w;
    
    for (i = 1; i < g->num_nodes; i++) {
        edge_ptr = g->edges;
        end_ptr = g->edges + g->num_edges;
        
        while (edge_ptr < end_ptr) {
            u = edge_ptr->src;
            v = edge_ptr->dest;
            w = edge_ptr->weight;
            
            if (dist[u] != 1000000 && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
            }
            edge_ptr++;
        }
    }
    
    edge_ptr = g->edges;
    end_ptr = g->edges + g->num_edges;
    while (edge_ptr < end_ptr) {
        u = edge_ptr->src;
        v = edge_ptr->dest;
        w = edge_ptr->weight;
        
        if (dist[u] != 1000000 && dist[u] + w < dist[v]) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
        edge_ptr++;
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (i = 0; i < g->num_nodes; i++) {
        if (dist[i] == 1000000) {
            printf("  Node %d: unreachable\n", i);
        } else {
            printf("  Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph g;
    int num_nodes, i;
    int src, dest, weight;
    int start_node;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1 || num_nodes <= 0 || num_nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    
    init_graph(&g, num_nodes);
    
    printf("Enter edges (source destination weight). Enter -1 -1 -1 to stop:\n");
    while (1) {
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid input format\n");
            return 1;
        }
        
        if (src == -1 && dest == -1 && weight == -1) {
            break;
        }
        
        if (!add_edge(&g, src, dest, weight)) {
            fprintf(stderr, "Failed to add edge\n");
            return 1;
        }
    }
    
    print_graph(&g);
    
    printf("Enter starting node for Bellman-Ford: ");
    if (scanf("%d", &start_node) != 1 || start_node < 0 || start_node >= g.num_nodes) {
        fprintf(stderr, "Invalid starting node\n");
        return 1;
    }
    
    bellman_ford(&g, start_node);
    
    return 0;
}