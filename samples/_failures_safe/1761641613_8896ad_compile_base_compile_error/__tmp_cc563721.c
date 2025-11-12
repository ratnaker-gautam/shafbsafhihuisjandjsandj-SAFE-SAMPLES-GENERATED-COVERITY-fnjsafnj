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
};

void initialize_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = 0;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

void add_edge(struct Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->num_nodes || v < 0 || v >= g->num_nodes) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    if (weight <= 0) {
        fprintf(stderr, "Invalid edge weight\n");
        exit(1);
    }
    g->adjacency[u][v] = weight;
    g->adjacency[v][u] = weight;
    g->num_edges++;
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
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!visited[v] && g->adjacency[min_node][v] != 0) {
                long new_dist = (long)distances[min_node] + g->adjacency[min_node][v];
                if (new_dist < distances[v] && new_dist <= INT_MAX) {
                    distances[v] = new_dist;
                }
            }
        }
    }
}

void print_distances(struct Graph *g, int source, int distances[]) {
    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < g->num_nodes; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

int read_int(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            exit(1);
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Please enter an integer between %d and %d.\n", min, max);
    }
}

int main() {
    struct Graph graph;
    int distances[MAX_NODES];
    
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n\n");
    
    int nodes = read_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    initialize_graph(&graph, nodes);
    
    int edges = read_int("Enter number of edges: ", 0, MAX_EDGES);
    
    printf("\nEnter edges (source destination weight):\n");
    for (int i = 0; i < edges; i++) {
        printf("Edge %d: ", i + 1);
        
        int u, v, w;
        char buffer[100];
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
        
        if (sscanf(buffer, "%d %d %d", &u, &v, &w) != 3) {
            printf("Invalid input format. Please enter: source destination weight\n");
            i--;
            continue;
        }
        
        if (u < 0 || u >= nodes || v < 0 || v >= nodes) {
            printf("Node indices must be between 0 and %d\n", nodes - 1);
            i--;
            continue;
        }
        
        if (w <= 0) {
            printf("Weight must be positive\n");
            i--;
            continue;
        }
        
        add_edge(&graph, u, v, w);
    }
    
    printf("\n");
    int source = read_int("Enter source node for shortest path calculation: ", 0, nodes - 1);
    
    dijkstra(&graph, source, distances);
    print_dist