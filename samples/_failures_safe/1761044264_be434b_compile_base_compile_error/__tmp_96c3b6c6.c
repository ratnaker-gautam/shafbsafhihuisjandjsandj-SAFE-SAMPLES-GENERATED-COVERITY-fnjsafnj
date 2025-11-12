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
    if (g->adjacency[from][to] != 0) {
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

void print_graph(struct Graph *g) {
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    for (int i = 0; i < g->num_nodes; i++) {
        printf("Node %d connects to: ", i);
        int has_edges = 0;
        for (int j = 0; j < g->num_nodes; j++) {
            if (g->adjacency[i][j]) {
                printf("%d(weight:%d) ", j, g->edge_weights[i][j]);
                has_edges = 1;
            }
        }
        if (!has_edges) {
            printf("none");
        }
        printf("\n");
    }
}

int main() {
    struct Graph g;
    initialize_graph(&g);
    
    printf("Graph Algorithm Demo - Dijkstra's Shortest Path\n");
    printf("Creating a sample graph with 6 nodes...\n");
    
    for (int i = 0; i < 6; i++) {
        if (add_node(&g) == -1) {
            printf("Error: Cannot add more nodes\n");
            return 1;
        }
    }
    
    int edges[][3] = {
        {0, 1, 4}, {0, 2, 2}, {1, 2, 1}, {1, 3, 5},
        {2, 3, 8}, {2, 4, 10}, {3, 4, 2}, {3, 5, 6},
        {4, 5, 3}
    };
    
    int num_edges = sizeof(edges) / sizeof(edges[0]);
    for (int i = 0; i < num_edges; i++) {
        if (add_edge(&g, edges[i][0], edges[i][1], edges[i][2]) == -1) {
            printf("Error: Cannot add edge %d->%d\n", edges[i][0], edges[i][1]);
            return 1;
        }
    }
    
    print_graph(&g);
    
    printf("\nEnter source node for Dijkstra (0-5): ");
    int source;
    if (scanf("%d", &source) != 1) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    if (source < 0 || source >= g.num_nodes) {
        printf("Error: Source node must be between 0 and %d\n", g.num_nodes - 1);
        return