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

void init_graph(struct Graph *g, int nodes) {
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid graph initialization\n");
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = 0;
}

int add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g == NULL || src < 0 || src >= g->num_nodes || 
        dest < 0 || dest >= g->num_nodes || g->num_edges >= MAX_EDGES) {
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
    if (g == NULL) return;
    
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    struct Edge *edge_ptr = g->edges;
    for (int i = 0; i < g->num_edges; i++) {
        printf("  %d -> %d (weight: %d)\n", edge_ptr->src, edge_ptr->dest, edge_ptr->weight);
        edge_ptr++;
    }
}

void dfs_util(struct Graph *g, int node, int *visited) {
    if (g == NULL || visited == NULL || node < 0 || node >= g->num_nodes) return;
    
    visited[node] = 1;
    printf("%d ", node);
    
    struct Edge *edge_ptr = g->edges;
    for (int i = 0; i < g->num_edges; i++) {
        if (edge_ptr->src == node && !visited[edge_ptr->dest]) {
            dfs_util(g, edge_ptr->dest, visited);
        }
        edge_ptr++;
    }
}

void dfs(struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->num_nodes) {
        printf("Invalid DFS start node\n");
        return;
    }
    
    int visited[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        visited[i] = 0;
    }
    
    printf("DFS traversal from node %d: ", start);
    dfs_util(g, start, visited);
    printf("\n");
}

int compare_edges(const void *a, const void *b) {
    const struct Edge *edge_a = (const struct Edge *)a;
    const struct Edge *edge_b = (const struct Edge *)b;
    return edge_a->weight - edge_b->weight;
}

int find_parent(int parent[], int node) {
    if (parent[node] != node) {
        parent[node] = find_parent(parent, parent[node]);
    }
    return parent[node];
}

void kruskal_mst(struct Graph *g) {
    if (g == NULL || g->num_nodes == 0) {
        printf("Invalid graph for MST\n");
        return;
    }
    
    struct Edge result[MAX_NODES];
    int result_count = 0;
    
    struct Edge sorted_edges[MAX_EDGES];
    struct Edge *src_ptr = g->edges;
    struct Edge *dest_ptr = sorted_edges;
    for (int i = 0; i < g->num_edges; i++) {
        *dest_ptr = *src_ptr;
        src_ptr++;
        dest_ptr++;
    }
    
    qsort(sorted_edges, g->num_edges, sizeof(struct Edge), compare_edges);
    
    int parent[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        parent[i] = i;
    }
    
    struct Edge *edge_ptr = sorted_edges;
    for (int i = 0; i < g->num_edges && result_count < g->num_nodes - 1; i++) {
        int src_parent = find_parent(parent, edge_ptr->src);
        int dest_parent = find_parent(parent, edge_ptr->dest);
        
        if (src_parent != dest_parent) {
            result[result_count] = *edge_ptr;
            result_count++;
            parent[src_parent] = dest_parent;
        }
        edge_ptr++;
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    struct Edge *result_ptr = result;
    for (int i = 0; i < result_count; i++) {
        printf("  %d - %d (weight: %d)\n", result_ptr->src, result_ptr->dest, result_ptr->weight);
        total_weight += result_ptr->weight;
        result_ptr++;
    }
    printf("Total MST weight: %d\n", total_weight);
}

int main() {
    struct Graph g;
    init_graph(&g, 6);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 4);
    add_edge(&g, 1,