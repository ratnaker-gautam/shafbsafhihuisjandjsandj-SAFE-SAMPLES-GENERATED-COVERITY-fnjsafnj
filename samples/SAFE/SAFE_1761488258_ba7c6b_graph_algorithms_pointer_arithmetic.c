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

void initialize_graph(struct Graph *graph) {
    graph->num_nodes = 0;
    graph->num_edges = 0;
}

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph->num_edges >= MAX_EDGES) return 0;
    if (src < 0 || dest < 0 || src >= MAX_NODES || dest >= MAX_NODES) return 0;
    
    struct Edge *edge_ptr = graph->edges + graph->num_edges;
    edge_ptr->src = src;
    edge_ptr->dest = dest;
    edge_ptr->weight = weight;
    graph->num_edges++;
    
    if (src >= graph->num_nodes) graph->num_nodes = src + 1;
    if (dest >= graph->num_nodes) graph->num_nodes = dest + 1;
    
    return 1;
}

void bellman_ford(struct Graph *graph, int start) {
    if (graph->num_nodes == 0) return;
    if (start < 0 || start >= graph->num_nodes) return;
    
    int distances[MAX_NODES];
    for (int i = 0; i < graph->num_nodes; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    for (int i = 0; i < graph->num_nodes - 1; i++) {
        struct Edge *edge_ptr = graph->edges;
        for (int j = 0; j < graph->num_edges; j++) {
            if (distances[edge_ptr->src] != INT_MAX) {
                int new_dist = distances[edge_ptr->src] + edge_ptr->weight;
                if (new_dist < distances[edge_ptr->dest]) {
                    distances[edge_ptr->dest] = new_dist;
                }
            }
            edge_ptr++;
        }
    }
    
    struct Edge *edge_ptr = graph->edges;
    for (int i = 0; i < graph->num_edges; i++) {
        if (distances[edge_ptr->src] != INT_MAX) {
            int new_dist = distances[edge_ptr->src] + edge_ptr->weight;
            if (new_dist < distances[edge_ptr->dest]) {
                printf("Graph contains negative weight cycle\n");
                return;
            }
        }
        edge_ptr++;
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Enter number of edges: ");
    int num_edges;
    if (scanf("%d", &num_edges) != 1 || num_edges <= 0 || num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&graph, src, dest, weight)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    printf("Enter starting node: ");
    int start;
    if (scanf("%d", &start) != 1) {
        printf("Invalid starting node\n");
        return 1;
    }
    
    bellman_ford(&graph, start);
    
    return 0;
}