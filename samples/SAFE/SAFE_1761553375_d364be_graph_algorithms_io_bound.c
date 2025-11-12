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

int validate_node(int node, int max_nodes) {
    return node >= 0 && node < max_nodes;
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

void dijkstra(struct Graph *g, int source) {
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[source] = 0;

    for (int count = 0; count < g->num_nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_node = v;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!visited[v] && g->adjacency[min_node][v] && dist[min_node] != INT_MAX) {
                int new_dist = dist[min_node] + g->edge_weights[min_node][v];
                if (new_dist < dist[v]) {
                    dist[v] = new_dist;
                }
            }
        }
    }

    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < g->num_nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph g;
    initialize_graph(&g);
    
    printf("Graph Shortest Path Calculator\n");
    if (!read_graph(&g)) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    int source;
    printf("Enter source node for shortest paths (0-%d): ", g.num_nodes - 1);
    if (scanf("%d", &source) != 1) {
        printf("Error: Invalid source node\n");
        return 1;
    }
    
    if (!validate_node(source, g.num_nodes)) {
        printf("Error: Invalid source node\n");
        return 1;
    }
    
    dijkstra(&g, source);
    return 0;
}