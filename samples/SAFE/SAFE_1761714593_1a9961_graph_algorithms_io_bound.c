//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g->adjacency[i][j] = 0;
            g->edge_weights[i][j] = 0;
        }
    }
}

int add_node(struct Graph *g) {
    if (g->num_nodes >= MAX_NODES) {
        return -1;
    }
    return g->num_nodes++;
}

int add_edge(struct Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->num_nodes || to < 0 || to >= g->num_nodes) {
        return -1;
    }
    if (weight < 0) {
        return -1;
    }
    if (g->adjacency[from][to] == 1) {
        return -1;
    }
    g->adjacency[from][to] = 1;
    g->edge_weights[from][to] = weight;
    g->num_edges++;
    return 0;
}

void dijkstra(struct Graph *g, int source, int distances[]) {
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->num_nodes; i++) {
        distances[i] = INT_MAX;
    }
    distances[source] = 0;
    
    for (int count = 0; count < g->num_nodes - 1; count++) {
        int min_distance = INT_MAX;
        int min_index = -1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!visited[v] && distances[v] <= min_distance) {
                min_distance = distances[v];
                min_index = v;
            }
        }
        
        if (min_index == -1) break;
        
        visited[min_index] = 1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!visited[v] && g->adjacency[min_index][v] && 
                distances[min_index] != INT_MAX) {
                long new_dist = (long)distances[min_index] + g->edge_weights[min_index][v];
                if (new_dist < distances[v] && new_dist <= INT_MAX) {
                    distances[v] = (int)new_dist;
                }
            }
        }
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Graph Construction\n");
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    
    int node_count;
    if (scanf("%d", &node_count) != 1 || node_count < 1 || node_count > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    for (int i = 0; i < node_count; i++) {
        add_node(&graph);
    }
    
    printf("Enter edges (from to weight), -1 -1 -1 to finish:\n");
    
    while (1) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid input format\n");
            return 1;
        }
        
        if (from == -1 && to == -1 && weight == -1) {
            break;
        }
        
        if (add_edge(&graph, from, to, weight) != 0) {
            printf("Invalid edge: %d -> %d (weight %d)\n", from, to, weight);
        }
    }
    
    printf("\nGraph Analysis\n");
    printf("Nodes: %d, Edges: %d\n", graph.num_nodes, graph.num_edges);
    
    printf("\nShortest Paths from Node 0:\n");
    int distances[MAX_NODES];
    dijkstra(&graph, 0, distances);
    
    for (int i = 0; i < graph.num_nodes; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: distance %d\n", i, distances[i]);
        }
    }
    
    printf("\nAdjacency Matrix:\n");
    for (int i = 0; i < graph.num_nodes; i++) {
        for (int j = 0; j < graph.num_nodes; j++) {
            if (graph.adjacency[i][j]) {
                printf("%d ", graph.edge_weights[i][j]);
            } else {
                printf("0 ");
            }
        }
        printf("\n");
    }
    
    return 0;
}