//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 500

struct Graph {
    int num_nodes;
    int num_edges;
    int adjacency[MAX_NODES][MAX_NODES];
    int edge_weights[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph *g) {
    if (g == NULL) return;
    
    g->num_nodes = 0;
    g->num_edges = 0;
    
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            *(*(g->adjacency + i) + j) = 0;
            *(*(g->edge_weights + i) + j) = 0;
        }
    }
}

int add_edge(struct Graph *g, int from, int to, int weight) {
    if (g == NULL) return 0;
    if (from < 0 || from >= MAX_NODES || to < 0 || to >= MAX_NODES) return 0;
    if (from == to) return 0;
    if (weight < 0) return 0;
    
    if (*(*(g->adjacency + from) + to) == 0) {
        *(*(g->adjacency + from) + to) = 1;
        *(*(g->edge_weights + from) + to) = weight;
        g->num_edges++;
        return 1;
    }
    return 0;
}

int add_node(struct Graph *g) {
    if (g == NULL) return -1;
    if (g->num_nodes >= MAX_NODES) return -1;
    
    int new_node = g->num_nodes;
    g->num_nodes++;
    return new_node;
}

void dijkstra(struct Graph *g, int source, int *distances) {
    if (g == NULL || distances == NULL) return;
    if (source < 0 || source >= g->num_nodes) return;
    
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->num_nodes; i++) {
        *(distances + i) = INT_MAX;
    }
    *(distances + source) = 0;
    
    for (int count = 0; count < g->num_nodes; count++) {
        int min_distance = INT_MAX;
        int min_node = -1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!*(visited + v) && *(distances + v) <= min_distance) {
                min_distance = *(distances + v);
                min_node = v;
            }
        }
        
        if (min_node == -1) break;
        *(visited + min_node) = 1;
        
        for (int neighbor = 0; neighbor < g->num_nodes; neighbor++) {
            if (!*(visited + neighbor) && *(*(g->adjacency + min_node) + neighbor)) {
                int edge_weight = *(*(g->edge_weights + min_node) + neighbor);
                if (*(distances + min_node) != INT_MAX) {
                    int new_distance = *(distances + min_node) + edge_weight;
                    if (new_distance > 0 && new_distance < *(distances + neighbor)) {
                        *(distances + neighbor) = new_distance;
                    }
                }
            }
        }
    }
}

void print_shortest_paths(struct Graph *g, int source) {
    if (g == NULL) return;
    if (source < 0 || source >= g->num_nodes) return;
    
    int distances[MAX_NODES];
    dijkstra(g, source, distances);
    
    printf("Shortest paths from node %d:\n", source);
    for (int i = 0; i < g->num_nodes; i++) {
        if (*(distances + i) == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: distance %d\n", i, *(distances + i));
        }
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    int node0 = add_node(&graph);
    int node1 = add_node(&graph);
    int node2 = add_node(&graph);
    int node3 = add_node(&graph);
    int node4 = add_node(&graph);
    
    if (node0 == -1 || node1 == -1 || node2 == -1 || node3 == -1 || node4 == -1) {
        printf("Failed to create nodes\n");
        return 1;
    }
    
    add_edge(&graph, node0, node1, 4);
    add_edge(&graph, node0, node2, 1);
    add_edge(&graph, node1, node3, 1);
    add_edge(&graph, node2, node1, 2);
    add_edge(&graph, node2, node3, 5);
    add_edge(&graph, node3, node4, 3);
    add_edge(&graph, node1, node4, 7);
    
    printf("Graph created with %d nodes and %d edges\n", graph.num_nodes, graph.num_edges);
    
    print_shortest_paths(&graph, node0);
    
    printf("\nRunning Dijk