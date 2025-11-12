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

void add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g->num_edges >= MAX_EDGES) {
        fprintf(stderr, "Too many edges\n");
        exit(1);
    }
    if (src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    struct Edge *e = g->edges + g->num_edges;
    e->src = src;
    e->dest = dest;
    e->weight = weight;
    g->num_edges++;
}

void bellman_ford(struct Graph *g, int start) {
    int dist[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = 1000000;
    }
    dist[start] = 0;

    for (int i = 0; i < g->num_nodes - 1; i++) {
        struct Edge *edge_ptr = g->edges;
        for (int j = 0; j < g->num_edges; j++) {
            if (dist[edge_ptr->src] != 1000000 && dist[edge_ptr->src] + edge_ptr->weight < dist[edge_ptr->dest]) {
                dist[edge_ptr->dest] = dist[edge_ptr->src] + edge_ptr->weight;
            }
            edge_ptr++;
        }
    }

    struct Edge *edge_ptr = g->edges;
    for (int i = 0; i < g->num_edges; i++) {
        if (dist[edge_ptr->src] != 1000000 && dist[edge_ptr->src] + edge_ptr->weight < dist[edge_ptr->dest]) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
        edge_ptr++;
    }

    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < g->num_nodes; i++) {
        printf("Node %d: %d\n", i, dist[i]);
    }
}

int main() {
    struct Graph g;
    int num_nodes, num_edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &num_nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    init_graph(&g, num_nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &num_edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (num_edges < 0 || num_edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as src dest weight:\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&g, src, dest, weight);
    }
    
    int start_node;
    printf("Enter starting node: ");
    if (scanf("%d", &start_node) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (start_node < 0 || start_node >= g.num_nodes) {
        fprintf(stderr, "Invalid starting node\n");
        return 1;
    }
    
    bellman_ford(&g, start_node);
    
    return 0;
}