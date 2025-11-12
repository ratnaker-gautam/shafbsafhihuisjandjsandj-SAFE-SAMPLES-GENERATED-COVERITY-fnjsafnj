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
    int adj_matrix[MAX_NODES][MAX_NODES];
};

void init_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            *(*(g->adj_matrix + i) + j) = 0;
        }
    }
}

int add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g->num_edges >= MAX_EDGES) return 0;
    if (src < 0 || src >= MAX_NODES || dest < 0 || dest >= MAX_NODES) return 0;
    if (src == dest) return 0;
    
    struct Edge *e = g->edges + g->num_edges;
    e->src = src;
    e->dest = dest;
    e->weight = weight;
    
    *(*(g->adj_matrix + src) + dest) = weight;
    *(*(g->adj_matrix + dest) + src) = weight;
    
    g->num_edges++;
    if (src >= g->num_nodes) g->num_nodes = src + 1;
    if (dest >= g->num_nodes) g->num_nodes = dest + 1;
    
    return 1;
}

void print_graph(struct Graph *g) {
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    for (int i = 0; i < g->num_nodes; i++) {
        printf("Node %d: ", i);
        for (int j = 0; j < g->num_nodes; j++) {
            int weight = *(*(g->adj_matrix + i) + j);
            if (weight != 0) {
                printf("->%d(%d) ", j, weight);
            }
        }
        printf("\n");
    }
}

void dfs_util(struct Graph *g, int node, int *visited) {
    *(visited + node) = 1;
    printf("%d ", node);
    
    for (int i = 0; i < g->num_nodes; i++) {
        int weight = *(*(g->adj_matrix + node) + i);
        if (weight != 0 && *(visited + i) == 0) {
            dfs_util(g, i, visited);
        }
    }
}

void dfs(struct Graph *g, int start) {
    if (start < 0 || start >= g->num_nodes) return;
    
    int visited[MAX_NODES];
    for (int i = 0; i < MAX_NODES; i++) {
        *(visited + i) = 0;
    }
    
    printf("DFS traversal from node %d: ", start);
    dfs_util(g, start, visited);
    printf("\n");
}

int compare_edges(const void *a, const void *b) {
    const struct Edge *ea = a;
    const struct Edge *eb = b;
    return ea->weight - eb->weight;
}

int find_parent(int *parent, int node) {
    if (*(parent + node) != node) {
        *(parent + node) = find_parent(parent, *(parent + node));
    }
    return *(parent + node);
}

void kruskal_mst(struct Graph *g) {
    if (g->num_nodes == 0) return;
    
    struct Edge result[MAX_NODES];
    int result_count = 0;
    
    int parent[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        *(parent + i) = i;
    }
    
    struct Edge edges_copy[MAX_EDGES];
    for (int i = 0; i < g->num_edges; i++) {
        *(edges_copy + i) = *(g->edges + i);
    }
    
    qsort(edges_copy, g->num_edges, sizeof(struct Edge), compare_edges);
    
    for (int i = 0; i < g->num_edges && result_count < g->num_nodes - 1; i++) {
        struct Edge *e = edges_copy + i;
        int src_parent = find_parent(parent, e->src);
        int dest_parent = find_parent(parent, e->dest);
        
        if (src_parent != dest_parent) {
            *(result + result_count) = *e;
            result_count++;
            *(parent + src_parent) = dest_parent;
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int i = 0; i < result_count; i++) {
        struct Edge *e = result + i;
        printf("%d - %d : %d\n", e->src, e->dest, e->weight);
        total_weight += e->weight;
    }
    printf("Total weight: %d\n", total_weight);
}

int main() {
    struct Graph g;
    init_graph(&g);
    
    add_edge(&g, 0, 1, 4);
    add