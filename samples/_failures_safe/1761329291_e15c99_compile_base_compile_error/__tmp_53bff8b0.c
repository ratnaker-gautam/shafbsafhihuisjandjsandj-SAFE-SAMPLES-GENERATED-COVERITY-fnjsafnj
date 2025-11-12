//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
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

int validate_node(struct Graph *g, int node) {
    return (node >= 0 && node < g->num_nodes);
}

int add_edge(struct Graph *g, int from, int to, int weight) {
    if (!validate_node(g, from) || !validate_node(g, to)) {
        return 0;
    }
    if (from == to) {
        return 0;
    }
    if (g->adjacency[from][to] == 1) {
        return 0;
    }
    if (g->num_edges >= MAX_EDGES) {
        return 0;
    }
    g->adjacency[from][to] = 1;
    g->edge_weights[from][to] = weight;
    g->num_edges++;
    return 1;
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
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n\n");
    
    graph.num_nodes = read_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    if (graph.num_nodes == -1) {
        return 1;
    }
    
    int num_edges = read_int("Enter number of edges: ", 0, MAX_EDGES);
    if (num_edges == -1) {
        return 1;
    }
    
    printf("\nEnter edges (from to weight):\n");
    for (int i = 0; i < num_edges; i++) {
        printf("Edge %d: ", i + 1);
        
        int from, to, weight;
        char buffer[100];
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (sscanf(buffer, "%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid format. Use: from_node to_node weight\n");
            i--;
            continue;
        }
        
        if (!add_edge(&graph, from, to, weight)) {
            printf("Invalid edge. Nodes must be 0-%d, no self-loops, no duplicates.\n", 
                   graph.num_nodes - 1);
            i--;
        }
    }
    
    printf("\nGraph created with %d nodes and %d edges.\n", 
           graph.num_nodes, graph.num_edges);
    
    while (1) {
        printf("\nMenu:\n");
        printf("1. Find shortest paths from a node\n");
        printf("2. Exit\n");
        
        int choice = read_int("Enter choice: ", 1, 2);
        if (choice == -1) {
            break;
        }
        
        if (choice == 2) {
            break;
        }
        
        int