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
    struct Edge *edge_ptr = g->edges;
    for (int i = 0; i < g->num_edges; i++) {
        printf("Edge %d: %d -> %d (weight %d)\n", i, edge_ptr->src, edge_ptr->dest, edge_ptr->weight);
        edge_ptr++;
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
        dist[i] = -1;
        prev[i] = -1;
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
        
        struct Edge *edge_ptr = g->edges;
        for (int i = 0; i < g->num_edges; i++) {
            if (edge_ptr->src == min_node) {
                int neighbor = edge_ptr->dest;
                int new_dist = dist[min_node] + edge_ptr->weight;
                if (dist[neighbor] == -1 || new_dist < dist[neighbor]) {
                    dist[neighbor] = new_dist;
                    prev[neighbor] = min_node;
                }
            }
            edge_ptr++;
        }
    }
    
    if (dist[end] == -1) {
        return 0;
    }
    
    int current = end;
    int length = 0;
    int temp_path[MAX_NODES];
    
    while (current != -1) {
        if (length >= MAX_NODES) {
            return 0;
        }
        temp_path[length] = current;
        length++;
        current = prev[current];
    }
    
    if (length > *path_length) {
        return 0;
    }
    
    for (int i = 0; i < length; i++) {
        path[i] = temp_path[length - 1 - i];
    }
    *path_length = length;
    
    return 1;
}

int main() {
    struct Graph g;
    init_graph(&g, 6, 0);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 2);
    add_edge(&g, 1, 2, 1);
    add_edge(&g, 1, 3, 5);
    add_edge(&g, 2, 3, 8);
    add_edge(&g, 2, 4, 10);
    add_edge(&g, 3, 4, 2);
    add_edge(&g, 3, 5, 6);
    add_edge(&g, 4, 5, 3);
    
    print_graph(&g);
    
    int path[MAX_NODES];
    int path_length = MAX_NODES;
    
    printf("\nFinding shortest path from node 0 to node 5:\n");
    if (find_shortest