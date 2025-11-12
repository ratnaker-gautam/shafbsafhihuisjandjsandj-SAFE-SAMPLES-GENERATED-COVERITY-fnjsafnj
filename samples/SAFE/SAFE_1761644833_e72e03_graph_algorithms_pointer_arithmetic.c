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

void init_graph(struct Graph *g, int nodes, int edges) {
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        return;
    }
    g->num_nodes = nodes;
    g->num_edges = edges;
    memset(g->edges, 0, sizeof(g->edges));
}

int add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g == NULL || src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes) {
        return -1;
    }
    if (g->num_edges >= MAX_EDGES) {
        return -1;
    }
    struct Edge *e = g->edges + g->num_edges;
    e->src = src;
    e->dest = dest;
    e->weight = weight;
    g->num_edges++;
    return 0;
}

void bellman_ford(struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->num_nodes) {
        return;
    }
    
    int dist[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    
    for (int i = 0; i < g->num_nodes - 1; i++) {
        for (int j = 0; j < g->num_edges; j++) {
            struct Edge *e = g->edges + j;
            if (dist[e->src] != INT_MAX && dist[e->src] + e->weight < dist[e->dest]) {
                dist[e->dest] = dist[e->src] + e->weight;
            }
        }
    }
    
    for (int j = 0; j < g->num_edges; j++) {
        struct Edge *e = g->edges + j;
        if (dist[e->src] != INT_MAX && dist[e->src] + e->weight < dist[e->dest]) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < g->num_nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int read_int(const char *prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char *endptr;
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d\n", min, max);
    }
}

int main() {
    struct Graph graph;
    int num_nodes, num_edges;
    
    printf("Graph Shortest Path (Bellman-Ford Algorithm)\n");
    printf("============================================\n");
    
    num_nodes = read_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    if (num_nodes == -1) {
        return 1;
    }
    
    num_edges = read_int("Enter number of edges (0-1000): ", 0, MAX_EDGES);
    if (num_edges == -1) {
        return 1;
    }
    
    init_graph(&graph, num_nodes, num_edges);
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        
        src = read_int("Source node: ", 0, num_nodes - 1);
        if (src == -1) return 1;
        
        dest = read_int("Destination node: ", 0, num_nodes - 1);
        if (dest == -1) return 1;
        
        weight = read_int("Weight: ", -10000, 10000);
        if (weight == -1) return 1;
        
        if (add_edge(&graph, src, dest, weight) != 0) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    int start_node = read_int("Enter starting node: ", 0, num_nodes - 1);
    if (start_node == -1) {
        return 1;
    }
    
    bellman_ford(&graph, start_node);
    
    return 0;
}