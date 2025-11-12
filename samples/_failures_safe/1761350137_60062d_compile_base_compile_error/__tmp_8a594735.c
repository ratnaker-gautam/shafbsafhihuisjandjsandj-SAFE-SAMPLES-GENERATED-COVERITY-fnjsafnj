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
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = 0;
}

void add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g->num_edges >= MAX_EDGES) {
        fprintf(stderr, "Too many edges\n");
        exit(1);
    }
    if (src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    struct Edge *edge_ptr = g->edges + g->num_edges;
    edge_ptr->src = src;
    edge_ptr->dest = dest;
    edge_ptr->weight = weight;
    g->num_edges++;
}

void print_graph(struct Graph *g) {
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    struct Edge *edge_ptr = g->edges;
    for (int i = 0; i < g->num_edges; i++) {
        printf("  %d -> %d (weight %d)\n", edge_ptr->src, edge_ptr->dest, edge_ptr->weight);
        edge_ptr++;
    }
}

int find_set(int *parent, int node) {
    if (parent[node] != node) {
        parent[node] = find_set(parent, parent[node]);
    }
    return parent[node];
}

void union_sets(int *parent, int *rank, int x, int y) {
    int root_x = find_set(parent, x);
    int root_y = find_set(parent, y);
    if (root_x != root_y) {
        if (*(rank + root_x) < *(rank + root_y)) {
            *(parent + root_x) = root_y;
        } else if (*(rank + root_x) > *(rank + root_y)) {
            *(parent + root_y) = root_x;
        } else {
            *(parent + root_y) = root_x;
            (*(rank + root_x))++;
        }
    }
}

int compare_edges(const void *a, const void *b) {
    const struct Edge *edge_a = (const struct Edge *)a;
    const struct Edge *edge_b = (const struct Edge *)b;
    return edge_a->weight - edge_b->weight;
}

void kruskal_mst(struct Graph *g) {
    if (g->num_nodes <= 0) {
        printf("Graph is empty\n");
        return;
    }
    
    struct Edge result[MAX_NODES];
    int result_count = 0;
    
    qsort(g->edges, g->num_edges, sizeof(struct Edge), compare_edges);
    
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    
    for (int i = 0; i < g->num_nodes; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
    
    struct Edge *edge_ptr = g->edges;
    for (int i = 0; i < g->num_edges && result_count < g->num_nodes - 1; i++) {
        int src_root = find_set(parent, edge_ptr->src);
        int dest_root = find_set(parent, edge_ptr->dest);
        
        if (src_root != dest_root) {
            *(result + result_count) = *edge_ptr;
            result_count++;
            union_sets(parent, rank, src_root, dest_root);
        }
        edge_ptr++;
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    struct Edge *result_ptr = result;
    for (int i = 0; i < result_count; i++) {
        printf("  %d -> %d (weight %d)\n", result_ptr->src, result_ptr->dest, result_ptr->weight);
        total_weight += result_ptr->weight;
        result_ptr++;
    }
    printf("Total weight: %d\n", total_weight);
}

int main() {
    struct Graph g;
    init_graph(&g, 6);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 4);
    add_edge(&g, 1, 2, 2);
    add_edge(&g, 1, 0, 4);
    add_edge(&g, 2, 0, 4);
    add_edge(&g, 2, 1, 2);
    add_edge(&g, 2, 3, 3);
    add_edge(&g, 2, 5, 2);
    add_edge(&g, 2, 4, 4);
    add_edge(&g, 3, 2, 3);
    add_edge(&g, 3, 4, 3);
    add_edge(&g, 4, 2, 4);
    add_edge(&g, 4, 3, 3);