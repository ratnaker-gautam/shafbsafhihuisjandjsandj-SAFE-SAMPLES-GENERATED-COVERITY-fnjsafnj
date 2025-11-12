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
                int new_distance = distances[min_node] + g->edge_weights[min_node][v];
                if (new_distance > 0 && new_distance < distances[v]) {
                    distances[v] = new_distance;
                }
            }
        }
    }
}

void print_graph_info(struct Graph *g) {
    printf("Graph has %d nodes and %d edges\n", g->num_nodes, g->num_edges);
    printf("Adjacency list:\n");
    for (int i = 0; i < g->num_nodes; i++) {
        printf("Node %d: ", i);
        int has_neighbors = 0;
        for (int j = 0; j < g->num_nodes; j++) {
            if (g->adjacency[i][j]) {
                printf("%d(w:%d) ", j, g->edge_weights[i][j]);
                has_neighbors = 1;
            }
        }
        if (!has_neighbors) {
            printf("no neighbors");
        }
        printf("\n");
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &graph.num_nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (graph.num_nodes < 1 || graph.num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges: ");
    int total_edges;
    if (scanf("%d", &total_edges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (total_edges < 0 || total_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'from to weight' (0-indexed nodes):\n");
    for (int i = 0; i < total_edges; i++) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (weight < 0) {
            printf("Negative weights not allowed\n");
            return 1;
        }
        if (!add_edge(&graph, from, to, weight)) {
            printf("Failed to add edge %d->%d\n", from, to);
            return 1;
        }
    }
    
    printf("\n");
    print_graph_info(&graph);
    
    printf("\nEnter source node for Dijkstra's algorithm: ");
    int source;
    if (scanf("%d", &source) != 1) {
        printf("Invalid input\n");
        return 1;