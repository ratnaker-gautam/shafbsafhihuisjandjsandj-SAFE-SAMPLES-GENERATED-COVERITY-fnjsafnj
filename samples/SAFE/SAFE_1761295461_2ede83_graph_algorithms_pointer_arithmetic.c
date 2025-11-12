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
    if (src < 0 || src >= MAX_NODES) return 0;
    if (dest < 0 || dest >= MAX_NODES) return 0;
    if (weight < 0) return 0;
    
    struct Edge *edge_ptr = graph->edges + graph->num_edges;
    edge_ptr->src = src;
    edge_ptr->dest = dest;
    edge_ptr->weight = weight;
    graph->num_edges++;
    
    if (src >= graph->num_nodes) graph->num_nodes = src + 1;
    if (dest >= graph->num_nodes) graph->num_nodes = dest + 1;
    
    return 1;
}

void bellman_ford(struct Graph *graph, int source, int *distances) {
    if (graph->num_nodes == 0) return;
    if (source < 0 || source >= graph->num_nodes) return;
    
    for (int i = 0; i < graph->num_nodes; i++) {
        *(distances + i) = INT_MAX;
    }
    *(distances + source) = 0;
    
    for (int i = 0; i < graph->num_nodes - 1; i++) {
        for (int j = 0; j < graph->num_edges; j++) {
            struct Edge *edge_ptr = graph->edges + j;
            int u = edge_ptr->src;
            int v = edge_ptr->dest;
            int weight = edge_ptr->weight;
            
            if (*(distances + u) != INT_MAX) {
                int new_dist = *(distances + u) + weight;
                if (new_dist < *(distances + v)) {
                    *(distances + v) = new_dist;
                }
            }
        }
    }
    
    for (int j = 0; j < graph->num_edges; j++) {
        struct Edge *edge_ptr = graph->edges + j;
        int u = edge_ptr->src;
        int v = edge_ptr->dest;
        int weight = edge_ptr->weight;
        
        if (*(distances + u) != INT_MAX && *(distances + u) + weight < *(distances + v)) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }
}

void print_distances(int *distances, int num_nodes, int source) {
    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < num_nodes; i++) {
        if (*(distances + i) == INT_MAX) {
            printf("Node %d: INFINITY\n", i);
        } else {
            printf("Node %d: %d\n", i, *(distances + i));
        }
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    int edges_to_add[][3] = {
        {0, 1, 4}, {0, 2, 2}, {1, 2, 3}, {1, 3, 2}, 
        {1, 4, 3}, {2, 1, 1}, {2, 3, 4}, {2, 4, 5}, 
        {3, 4, 1}, {4, 3, 2}
    };
    
    int num_edges = sizeof(edges_to_add) / sizeof(edges_to_add[0]);
    
    for (int i = 0; i < num_edges; i++) {
        int *edge_data = *(edges_to_add + i);
        if (!add_edge(&graph, *edge_data, *(edge_data + 1), *(edge_data + 2))) {
            printf("Failed to add edge %d\n", i);
            return 1;
        }
    }
    
    int distances[MAX_NODES];
    int source_node = 0;
    
    bellman_ford(&graph, source_node, distances);
    print_distances(distances, graph.num_nodes, source_node);
    
    printf("\nTesting with different source node:\n");
    source_node = 3;
    bellman_ford(&graph, source_node, distances);
    print_distances(distances, graph.num_nodes, source_node);
    
    return 0;
}