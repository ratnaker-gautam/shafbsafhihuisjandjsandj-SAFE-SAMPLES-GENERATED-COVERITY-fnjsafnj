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

void bellman_ford(struct Graph *graph, int start, int *distances) {
    for (int i = 0; i < graph->num_nodes; i++) {
        *(distances + i) = 1000000;
    }
    *(distances + start) = 0;
    
    for (int i = 0; i < graph->num_nodes - 1; i++) {
        struct Edge *edge_ptr = graph->edges;
        for (int j = 0; j < graph->num_edges; j++) {
            int u = edge_ptr->src;
            int v = edge_ptr->dest;
            int weight = edge_ptr->weight;
            
            if (*(distances + u) != 1000000 && *(distances + u) + weight < *(distances + v)) {
                *(distances + v) = *(distances + u) + weight;
            }
            edge_ptr++;
        }
    }
    
    struct Edge *edge_ptr = graph->edges;
    for (int j = 0; j < graph->num_edges; j++) {
        int u = edge_ptr->src;
        int v = edge_ptr->dest;
        int weight = edge_ptr->weight;
        
        if (*(distances + u) != 1000000 && *(distances + u) + weight < *(distances + v)) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
        edge_ptr++;
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    int num_edges;
    printf("Enter number of edges: ");
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
    
    int start_node;
    printf("Enter start node: ");
    if (scanf("%d", &start_node) != 1 || start_node < 0 || start_node >= graph.num_nodes) {
        printf("Invalid start node\n");
        return 1;
    }
    
    int distances[MAX_NODES];
    bellman_ford(&graph, start_node, distances);
    
    printf("Shortest distances from node %d:\n", start_node);
    for (int i = 0; i < graph.num_nodes; i++) {
        if (*(distances + i) == 1000000) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, *(distances + i));
        }
    }
    
    return 0;
}