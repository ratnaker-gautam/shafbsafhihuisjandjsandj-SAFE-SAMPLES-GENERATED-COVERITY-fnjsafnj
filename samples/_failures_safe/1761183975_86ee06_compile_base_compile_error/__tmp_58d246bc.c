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
        int min_node = -1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!visited[v] && distances[v] <= min_distance) {
                min_distance = distances[v];
                min_node = v;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!visited[v] && g->adjacency[min_node][v] && 
                distances[min_node] != INT_MAX) {
                long new_dist = (long)distances[min_node] + g->edge_weights[min_node][v];
                if (new_dist < distances[v] && new_dist <= INT_MAX) {
                    distances[v] = (int)new_dist;
                }
            }
        }
    }
}

void print_graph(struct Graph *g) {
    printf("Graph has %d nodes and %d edges\n", g->num_nodes, g->num_edges);
    printf("Edges:\n");
    for (int i = 0; i < g->num_nodes; i++) {
        for (int j = 0; j < g->num_nodes; j++) {
            if (g->adjacency[i][j]) {
                printf("  %d -> %d (weight: %d)\n", i, j, g->edge_weights[i][j]);
            }
        }
    }
}

int main() {
    struct Graph g;
    initialize_graph(&g);
    
    printf("Graph Shortest Path Calculator\n");
    printf("Creating sample graph...\n");
    
    for (int i = 0; i < 6; i++) {
        if (add_node(&g) == -1) {
            printf("Failed to add node\n");
            return 1;
        }
    }
    
    if (add_edge(&g, 0, 1, 4) == -1) return 1;
    if (add_edge(&g, 0, 2, 2) == -1) return 1;
    if (add_edge(&g, 1, 2, 1) == -1) return 1;
    if (add_edge(&g, 1, 3, 5) == -1) return 1;
    if (add_edge(&g, 2, 3, 8) == -1) return 1;
    if (add_edge(&g, 2, 4, 10) == -1) return 1;
    if (add_edge(&g, 3, 4, 2) == -1) return 1;
    if (add_edge(&g, 3, 5, 6) == -1) return 1;
    if (add_edge(&g, 4, 5, 3) == -1) return 1;
    
    print_graph(&g);
    
    printf("\nEnter source node (0-%d): ", g.num_nodes - 1);
    int source;
    if (scanf("%d", &source) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (source < 0 || source >= g.num_nodes) {
        printf("Invalid node\n");
        return 1;
    }
    
    int distances[MAX_NODES];
    dijkstra(&g, source, distances);
    
    printf("\nShortest distances from node %d:\n", source);
    for (int i = 0; i