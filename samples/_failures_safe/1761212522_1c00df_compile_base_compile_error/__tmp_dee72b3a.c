//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Edge {
    int src;
    int dest;
    int weight;
};

struct Graph {
    int num_nodes;
    int num_edges;
    struct Edge edges[MAX_EDGES];
};

void init_graph(struct Graph *g, int nodes, int edges) {
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = edges;
    memset(g->edges, 0, sizeof(g->edges));
}

int add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g == NULL || src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes) {
        return 0;
    }
    if (g->num_edges >= MAX_EDGES) {
        return 0;
    }
    struct Edge *e = g->edges + g->num_edges;
    e->src = src;
    e->dest = dest;
    e->weight = weight;
    g->num_edges++;
    return 1;
}

void print_graph(struct Graph *g) {
    if (g == NULL) {
        return;
    }
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    for (int i = 0; i < g->num_edges; i++) {
        struct Edge *e = g->edges + i;
        printf("Edge %d: %d -> %d (weight %d)\n", i, e->src, e->dest, e->weight);
    }
}

void dijkstra(struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->num_nodes) {
        fprintf(stderr, "Invalid parameters for Dijkstra\n");
        return;
    }

    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = -1;
        visited[i] = 0;
    }
    dist[start] = 0;

    for (int count = 0; count < g->num_nodes - 1; count++) {
        int min_dist = -1;
        int min_node = -1;
        
        for (int i = 0; i < g->num_nodes; i++) {
            if (!visited[i] && dist[i] != -1 && (min_dist == -1 || dist[i] < min_dist)) {
                min_dist = dist[i];
                min_node = i;
            }
        }
        
        if (min_node == -1) {
            break;
        }
        
        visited[min_node] = 1;
        
        for (int i = 0; i < g->num_edges; i++) {
            struct Edge *e = g->edges + i;
            if (e->src == min_node) {
                int new_dist = dist[min_node] + e->weight;
                if (dist[e->dest] == -1 || new_dist < dist[e->dest]) {
                    dist[e->dest] = new_dist;
                }
            }
        }
    }

    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < g->num_nodes; i++) {
        if (dist[i] != -1) {
            printf("Node %d: %d\n", i, dist[i]);
        } else {
            printf("Node %d: unreachable\n", i);
        }
    }
}

int main() {
    struct Graph g;
    int num_nodes, num_edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1 || num_nodes < 1 || num_nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    init_graph(&g, num_nodes, num_edges);
    
    printf("Enter edges as 'source destination weight' (0-indexed):\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&g, src, dest, weight)) {
            fprintf(stderr, "Failed to add edge\n");
            return 1;
        }
    }
    
    print_graph(&g);
    
    int start_node;
    printf("Enter starting node for Dijkstra (0-%d): ", num_nodes - 1);
    if (scanf("%d", &start_node) != 1 || start_node < 0 || start_node >= num_nodes)