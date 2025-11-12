//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 5000

struct Graph {
    int num_nodes;
    int num_edges;
    int adjacency[MAX_NODES][MAX_NODES];
    int edge_weights[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g->adjacency[i][j] = 0;
            g->edge_weights[i][j] = 0;
        }
    }
}

int validate_node(int node, int max_nodes) {
    return (node >= 0 && node < max_nodes);
}

int read_graph(struct Graph *g) {
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &g->num_nodes) != 1) {
        return 0;
    }
    if (g->num_nodes < 1 || g->num_nodes > MAX_NODES) {
        return 0;
    }

    printf("Enter number of edges: ");
    if (scanf("%d", &g->num_edges) != 1) {
        return 0;
    }
    if (g->num_edges < 0 || g->num_edges > MAX_EDGES) {
        return 0;
    }

    printf("Enter edges as 'source destination weight' (nodes 0-%d):\n", g->num_nodes - 1);
    for (int i = 0; i < g->num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            return 0;
        }
        if (!validate_node(src, g->num_nodes) || !validate_node(dest, g->num_nodes)) {
            return 0;
        }
        if (src == dest) {
            return 0;
        }
        if (weight < 0) {
            return 0;
        }
        g->adjacency[src][dest] = 1;
        g->edge_weights[src][dest] = weight;
    }
    return 1;
}

void dijkstra(struct Graph *g, int source, int distances[]) {
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->num_nodes; i++) {
        distances[i] = INT_MAX;
    }
    distances[source] = 0;
    
    for (int count = 0; count < g->num_nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!visited[v] && distances[v] <= min_dist) {
                min_dist = distances[v];
                min_node = v;
            }
        }
        
        if (min_node == -1) {
            break;
        }
        
        visited[min_node] = 1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!visited[v] && g->adjacency[min_node][v] && distances[min_node] != INT_MAX) {
                int new_dist = distances[min_node] + g->edge_weights[min_node][v];
                if (new_dist > 0 && new_dist < distances[v]) {
                    distances[v] = new_dist;
                }
            }
        }
    }
}

void print_shortest_paths(struct Graph *g, int source, int distances[]) {
    printf("\nShortest paths from node %d:\n", source);
    for (int i = 0; i < g->num_nodes; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: distance %d\n", i, distances[i]);
        }
    }
}

int main() {
    struct Graph graph;
    int distances[MAX_NODES];
    int source_node;
    
    initialize_graph(&graph);
    
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n");
    
    if (!read_graph(&graph)) {
        printf("Error: Invalid input data\n");
        return 1;
    }
    
    printf("\nEnter source node (0-%d): ", graph.num_nodes - 1);
    if (scanf("%d", &source_node) != 1) {
        printf("Error: Invalid source node\n");
        return 1;
    }
    
    if (!validate_node(source_node, graph.num_nodes)) {
        printf("Error: Invalid source node\n");
        return 1;
    }
    
    dijkstra(&graph, source_node, distances);
    print_shortest_paths(&graph, source_node, distances);
    
    return 0;
}