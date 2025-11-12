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
    memset(g->edges, 0, sizeof(g->edges[0]) * edges);
}

int add_edge(struct Graph *g, int idx, int src, int dest, int weight) {
    if (g == NULL || idx < 0 || idx >= g->num_edges) return 0;
    if (src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes) return 0;
    
    struct Edge *e = g->edges + idx;
    e->src = src;
    e->dest = dest;
    e->weight = weight;
    return 1;
}

void print_graph(struct Graph *g) {
    if (g == NULL) return;
    
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    for (int i = 0; i < g->num_edges; i++) {
        struct Edge *e = g->edges + i;
        printf("Edge %d: %d -> %d (weight %d)\n", i, e->src, e->dest, e->weight);
    }
}

int find_shortest_path(struct Graph *g, int start, int end, int *path, int *path_len) {
    if (g == NULL || path == NULL || path_len == NULL) return 0;
    if (start < 0 || start >= g->num_nodes || end < 0 || end >= g->num_nodes) return 0;
    
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
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int i = 0; i < g->num_edges; i++) {
            struct Edge *e = g->edges + i;
            if (e->src == min_node) {
                int new_dist = dist[min_node] + e->weight;
                if (dist[e->dest] == -1 || new_dist < dist[e->dest]) {
                    dist[e->dest] = new_dist;
                    prev[e->dest] = min_node;
                }
            }
        }
    }
    
    if (dist[end] == -1) return 0;
    
    int temp_path[MAX_NODES];
    int temp_len = 0;
    int current = end;
    
    while (current != -1) {
        if (temp_len >= MAX_NODES) return 0;
        *(temp_path + temp_len) = current;
        temp_len++;
        current = prev[current];
    }
    
    for (int i = 0; i < temp_len; i++) {
        if (i >= MAX_NODES) return 0;
        *(path + i) = *(temp_path + temp_len - 1 - i);
    }
    *path_len = temp_len;
    
    return 1;
}

int main() {
    struct Graph g;
    init_graph(&g, 6, 9);
    
    add_edge(&g, 0, 0, 1, 4);
    add_edge(&g, 1, 0, 2, 2);
    add_edge(&g, 2, 1, 2, 1);
    add_edge(&g, 3, 1, 3, 5);
    add_edge(&g, 4, 2, 3, 8);
    add_edge(&g, 5, 2, 4, 10);
    add_edge(&g, 6, 3, 4, 2);
    add_edge(&g, 7, 3, 5, 6);
    add_edge(&g, 8, 4, 5, 3);
    
    print_graph(&g);
    
    int path[MAX_NODES];
    int path_len;
    
    printf("\nFinding shortest path from node 0 to node 5:\n");
    if (find_shortest_path(&g, 0, 5, path, &path_len)) {