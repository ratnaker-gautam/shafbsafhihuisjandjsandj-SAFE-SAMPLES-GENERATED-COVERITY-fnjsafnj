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
        fprintf(stderr, "Invalid node indices\n");
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
        for (int i = 0; i < g->num_nodes; i++) {
            if (!visited[i] && distances[i] < min_dist) {
                min_dist = distances[i];
                min_node = i;
            }
        }
        if (min_node == -1) break;
        visited[min_node] = 1;
        for (int i = 0; i < g->num_nodes; i++) {
            if (!visited[i] && g->adjacency[min_node][i] != 0) {
                long new_dist = (long)distances[min_node] + g->adjacency[min_node][i];
                if (new_dist < distances[i] && new_dist < INT_MAX) {
                    distances[i] = (int)new_dist;
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

int main() {
    struct Graph g;
    int nodes, edges, source;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        fprintf(stderr, "Failed to read number of nodes\n");
        return 1;
    }
    if (nodes < 1 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    
    initialize_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        fprintf(stderr, "Failed to read number of edges\n");
        return 1;
    }
    if (edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            fprintf(stderr, "Failed to read edge\n");
            return 1;
        }
        if (u < 0 || u >= nodes || v < 0 || v >= nodes) {
            fprintf(stderr, "Invalid node in edge\n");
            return 1;
        }
        if (weight <= 0) {
            fprintf(stderr, "Invalid edge weight\n");
            return 1;
        }
        add_edge(&g, u, v, weight);
    }
    
    printf("Enter source node: ");
    if (scanf("%d", &source) != 1) {
        fprintf(stderr, "Failed to read source node\n");
        return 1;
    }
    if (source < 0 || source >= nodes) {
        fprintf(stderr, "Invalid source node\n");
        return 1;
    }
    
    int distances[MAX_NODES];
    dijkstra(&g, source, distances);
    print_distances(&g, source, distances);
    
    return 0;
}