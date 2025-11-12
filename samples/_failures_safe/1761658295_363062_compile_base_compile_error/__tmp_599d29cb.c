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
        return;
    }
    g->num_nodes = nodes;
    g->num_edges = edges;
    for (int i = 0; i < edges; i++) {
        (g->edges + i)->src = 0;
        (g->edges + i)->dest = 0;
        (g->edges + i)->weight = 0;
    }
}

int add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g == NULL || src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes || g->num_edges >= MAX_EDGES) {
        return 0;
    }
    struct Edge *edge_ptr = g->edges + g->num_edges;
    edge_ptr->src = src;
    edge_ptr->dest = dest;
    edge_ptr->weight = weight;
    g->num_edges++;
    return 1;
}

void print_graph(struct Graph *g) {
    if (g == NULL) {
        return;
    }
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    for (int i = 0; i < g->num_edges; i++) {
        struct Edge *edge_ptr = g->edges + i;
        printf("Edge %d: %d -> %d (weight %d)\n", i, edge_ptr->src, edge_ptr->dest, edge_ptr->weight);
    }
}

int find_shortest_path(struct Graph *g, int start, int end, int *path, int *path_length) {
    if (g == NULL || start < 0 || start >= g->num_nodes || end < 0 || end >= g->num_nodes || path == NULL || path_length == NULL) {
        return 0;
    }
    
    int dist[MAX_NODES];
    int prev[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = 1000000;
        prev[i] = -1;
        visited[i] = 0;
    }
    
    dist[start] = 0;
    
    for (int count = 0; count < g->num_nodes - 1; count++) {
        int min_dist = 1000000;
        int u = -1;
        
        for (int i = 0; i < g->num_nodes; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                u = i;
            }
        }
        
        if (u == -1 || u == end) {
            break;
        }
        
        visited[u] = 1;
        
        for (int i = 0; i < g->num_edges; i++) {
            struct Edge *edge_ptr = g->edges + i;
            if (edge_ptr->src == u) {
                int v = edge_ptr->dest;
                int alt = dist[u] + edge_ptr->weight;
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                }
            }
        }
    }
    
    if (dist[end] == 1000000) {
        return 0;
    }
    
    int temp_path[MAX_NODES];
    int temp_length = 0;
    int current = end;
    
    while (current != -1) {
        if (temp_length >= MAX_NODES) {
            return 0;
        }
        *(temp_path + temp_length) = current;
        temp_length++;
        current = prev[current];
    }
    
    if (temp_length > *path_length) {
        return 0;
    }
    
    for (int i = 0; i < temp_length; i++) {
        *(path + i) = *(temp_path + temp_length - 1 - i);
    }
    *path_length = temp_length;
    
    return 1;
}

int main() {
    struct Graph g;
    int num_nodes, num_edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &num_nodes) != 1 || num_nodes < 1 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges (0-1000): ");
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    init_graph(&g, num_nodes, num_edges);
    
    printf("Enter edges as 'source destination weight' (0-indexed):\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d