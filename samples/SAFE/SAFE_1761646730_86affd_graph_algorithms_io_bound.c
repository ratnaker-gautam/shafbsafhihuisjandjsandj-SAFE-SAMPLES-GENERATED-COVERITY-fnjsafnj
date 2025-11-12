//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 500

struct Graph {
    int nodes;
    int edges[MAX_EDGES][3];
    int edge_count;
};

void init_graph(struct Graph *g, int nodes) {
    if (nodes < 1 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->nodes = nodes;
    g->edge_count = 0;
}

int add_edge(struct Graph *g, int u, int v, int weight) {
    if (g->edge_count >= MAX_EDGES) {
        return 0;
    }
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        return 0;
    }
    if (weight < 0 || weight > 1000) {
        return 0;
    }
    g->edges[g->edge_count][0] = u;
    g->edges[g->edge_count][1] = v;
    g->edges[g->edge_count][2] = weight;
    g->edge_count++;
    return 1;
}

void dijkstra(struct Graph *g, int source, int dist[]) {
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[source] = 0;
    
    for (int count = 0; count < g->nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_node = v;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i][0] == min_node) {
                int v = g->edges[i][1];
                int weight = g->edges[i][2];
                if (!visited[v] && dist[min_node] != INT_MAX && 
                    dist[min_node] + weight < dist[v]) {
                    dist[v] = dist[min_node] + weight;
                }
            }
        }
    }
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    init_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'u v weight' (0-indexed):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&g, u, v, weight)) {
            fprintf(stderr, "Failed to add edge\n");
            return 1;
        }
    }
    
    int source;
    printf("Enter source node: ");
    if (scanf("%d", &source) != 1) {
        fprintf(stderr, "Invalid source node\n");
        return 1;
    }
    
    if (source < 0 || source >= nodes) {
        fprintf(stderr, "Invalid source node\n");
        return 1;
    }
    
    int dist[MAX_NODES];
    dijkstra(&g, source, dist);
    
    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
    
    return 0;
}