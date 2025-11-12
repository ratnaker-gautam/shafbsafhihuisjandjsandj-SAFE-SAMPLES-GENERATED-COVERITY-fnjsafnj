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
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        exit(1);
    }
    g->adjacency[u][v] = weight;
    g->adjacency[v][u] = weight;
    g->num_edges++;
}

void dijkstra(struct Graph *g, int source, int distances[]) {
    int visited[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        distances[i] = INT_MAX;
        visited[i] = 0;
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
                    distances[v] = (int)new_dist;
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
    int num_nodes, num_edges;
    int distances[MAX_NODES];
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    if (num_nodes <= 0 || num_nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    
    initialize_graph(&g, num_nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &num_edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    if (num_edges < 0 || num_edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < num_edges; i++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (u < 0 || u >= num_nodes || v < 0 || v >= num_nodes) {
            fprintf(stderr, "Invalid node index in edge\n");
            return 1;
        }
        if (weight < 0) {
            fprintf(stderr, "Negative weight not allowed\n");
            return 1;
        }
        add_edge(&g, u, v, weight);
    }
    
    int source;
    printf("Enter source node: ");
    if (scanf("%d", &source) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    if (source < 0 || source >= num_nodes) {
        fprintf(stderr, "Invalid source node\n");
        return 1;
    }
    
    dijkstra(&g, source, distances);
    print_distances(&g, source, distances);
    
    return 0;
}