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

void add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g == NULL || src < 0 || src >= g->num_nodes || 
        dest < 0 || dest >= g->num_nodes || 
        g->num_edges >= MAX_EDGES) {
        fprintf(stderr, "Invalid edge parameters\n");
        exit(1);
    }
    
    struct Edge *edge_ptr = g->edges + g->num_edges;
    edge_ptr->src = src;
    edge_ptr->dest = dest;
    edge_ptr->weight = weight;
    g->num_edges++;
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

void union_sets(int parent[], int rank[], int x, int y) {
    int root_x = find_parent(parent, x);
    int root_y = find_parent(parent, y);
    
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

void kruskal_mst(struct Graph *g) {
    if (g == NULL || g->num_nodes <= 0) {
        fprintf(stderr, "Invalid graph for MST\n");
        return;
    }
    
    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    
    for (int i = 0; i < g->num_nodes; i++) {
        *(parent + i) = i;
        *(rank + i) = 0;
    }
    
    qsort(g->edges, g->num_edges, sizeof(struct Edge), compare_edges);
    
    int result_count = 0;
    int edge_index = 0;
    
    while (result_count < g->num_nodes - 1 && edge_index < g->num_edges) {
        struct Edge *next_edge = g->edges + edge_index;
        edge_index++;
        
        int x = find_parent(parent, next_edge->src);
        int y = find_parent(parent, next_edge->dest);
        
        if (x != y) {
            *(result + result_count) = *next_edge;
            result_count++;
            union_sets(parent, rank, x, y);
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int i = 0; i < result_count; i++) {
        struct Edge *edge_ptr = result + i;
        printf("%d -- %d : %d\n", edge_ptr->src, edge_ptr->dest, edge_ptr->weight);
        total_weight += edge_ptr->weight;
    }
    printf("Total weight: %d\n", total_weight);
}

int main() {
    struct Graph graph;
    int num_nodes, num_edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1 || num_nodes < 1 || num_nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    
    init_graph(&graph, num_nodes);
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as: source destination weight\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (src < 0 || src >= num_nodes || dest < 0 || dest >= num_nodes) {
            fprintf(stderr, "Invalid node indices\n");
            return 1;
        }
        add_edge(&graph, src, dest, weight);
    }
    
    kruskal_mst(&graph);
    
    return 0;
}