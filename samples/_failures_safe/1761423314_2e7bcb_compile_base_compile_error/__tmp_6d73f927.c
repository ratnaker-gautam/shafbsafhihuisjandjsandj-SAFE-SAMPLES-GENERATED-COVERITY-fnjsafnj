//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int nodes[MAX_NODES];
    int edges[MAX_EDGES][3];
    int node_count;
    int edge_count;
};

void init_graph(struct Graph* g) {
    g->node_count = 0;
    g->edge_count = 0;
}

int add_node(struct Graph* g, int node) {
    if (g->node_count >= MAX_NODES) return 0;
    if (node < 0 || node >= MAX_NODES) return 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i] == node) return 0;
    }
    g->nodes[g->node_count++] = node;
    return 1;
}

int add_edge(struct Graph* g, int from, int to, int weight) {
    if (g->edge_count >= MAX_EDGES) return 0;
    if (from < 0 || from >= MAX_NODES || to < 0 || to >= MAX_NODES) return 0;
    if (weight < 0) return 0;
    
    int from_exists = 0, to_exists = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i] == from) from_exists = 1;
        if (g->nodes[i] == to) to_exists = 1;
    }
    if (!from_exists || !to_exists) return 0;
    
    g->edges[g->edge_count][0] = from;
    g->edges[g->edge_count][1] = to;
    g->edges[g->edge_count][2] = weight;
    g->edge_count++;
    return 1;
}

void print_graph(struct Graph* g) {
    printf("Graph nodes: ");
    for (int i = 0; i < g->node_count; i++) {
        printf("%d", g->nodes[i]);
        if (i < g->node_count - 1) printf(", ");
    }
    printf("\n");
    
    printf("Graph edges:\n");
    for (int i = 0; i < g->edge_count; i++) {
        printf("  %d -> %d (weight: %d)\n", 
               g->edges[i][0], g->edges[i][1], g->edges[i][2]);
    }
}

int find_shortest_path(struct Graph* g, int start, int end) {
    if (start < 0 || start >= MAX_NODES || end < 0 || end >= MAX_NODES) return -1;
    
    int start_exists = 0, end_exists = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i] == start) start_exists = 1;
        if (g->nodes[i] == end) end_exists = 1;
    }
    if (!start_exists || !end_exists) return -1;
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < MAX_NODES; i++) {
        dist[i] = -1;
        visited[i] = 0;
    }
    
    dist[start] = 0;
    
    for (int count = 0; count < g->node_count; count++) {
        int min_dist = -1;
        int min_node = -1;
        
        for (int i = 0; i < g->node_count; i++) {
            int node = g->nodes[i];
            if (!visited[node] && dist[node] != -1) {
                if (min_dist == -1 || dist[node] < min_dist) {
                    min_dist = dist[node];
                    min_node = node;
                }
            }
        }
        
        if (min_node == -1) break;
        if (min_node == end) break;
        
        visited[min_node] = 1;
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i][0] == min_node) {
                int neighbor = g->edges[i][1];
                int weight = g->edges[i][2];
                int new_dist = dist[min_node] + weight;
                
                if (dist[neighbor] == -1 || new_dist < dist[neighbor]) {
                    dist[neighbor] = new_dist;
                }
            }
        }
    }
    
    return dist[end];
}

int main() {
    struct Graph g;
    init_graph(&g);
    
    printf("Graph Shortest Path Finder\n");
    printf("Enter nodes (0-99, -1 to finish):\n");
    
    int node;
    while (1) {
        if (scanf("%d", &node) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        if (node == -1) break;
        if (!add_node(&g, node)) {
            printf("Invalid node or graph full\n");
        }
    }
    
    printf("Enter edges (from to weight, -1 -1 -1 to finish):\n");
    int from, to, weight;
    while (1) {
        if (scanf("%d %d %d", &from, &to, &