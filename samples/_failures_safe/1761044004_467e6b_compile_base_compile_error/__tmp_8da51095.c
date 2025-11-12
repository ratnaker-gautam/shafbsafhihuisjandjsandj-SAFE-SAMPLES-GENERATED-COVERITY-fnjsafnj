//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
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
    if (g->num_edges >= MAX_EDGES) {
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

int read_int(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return value;
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n\n");
    
    int node_count = read_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    if (node_count == -1) {
        return 1;
    }
    
    for (int i = 0; i < node_count; i++) {
        add_node(&graph);
    }
    
    printf("\nEnter edges (from, to, weight). Enter -1 to finish.\n");
    
    while (1) {
        int from = read_int("From node (0-%d, -1 to finish): ", -1, node_count - 1);
        if (from == -1) break;
        
        int to = read_int("To node (0-%d): ", 0, node_count - 1);
        int weight = read_int("Weight (1-1000): ", 1, 1000);
        
        if (add_edge(&graph, from, to, weight) == -1) {
            printf("Failed to add edge. Edge may already exist or limit reached.\n");
        } else {
            printf("Edge added successfully.\n");
        }
    }
    
    printf("\nComputing shortest paths...\n");
    
    int source = read_int("Enter source node for shortest paths (0-%d): ", 0, node_count - 1);
    
    int distances[MAX_NODES];
    dijkstra(&graph, source, distances);
    
    printf("\nShortest paths from node %d:\n", source);
    printf("Node\tDistance\n");
    printf("----\t--------\n");
    
    for (int i = 0; i < graph.num_nodes; i++) {
        if (distances[i] == INT_MAX) {
            printf("%d\tUn