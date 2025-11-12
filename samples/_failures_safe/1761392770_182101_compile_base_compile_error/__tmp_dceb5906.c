//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000
#define MAX_LINE 256

typedef struct {
    int from;
    int to;
    int weight;
} Edge;

typedef struct {
    int node_count;
    int edge_count;
    Edge edges[MAX_EDGES];
} Graph;

int read_graph(Graph *graph) {
    char line[MAX_LINE];
    int node_count, edge_count;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    if (sscanf(line, "%d", &node_count) != 1) return 0;
    if (node_count < 1 || node_count > MAX_NODES) return 0;
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    if (sscanf(line, "%d", &edge_count) != 1) return 0;
    if (edge_count < 0 || edge_count > MAX_EDGES) return 0;
    
    graph->node_count = node_count;
    graph->edge_count = 0;
    
    for (int i = 0; i < edge_count; i++) {
        int from, to, weight;
        printf("Enter edge %d (from to weight): ", i + 1);
        if (fgets(line, sizeof(line), stdin) == NULL) return 0;
        if (sscanf(line, "%d %d %d", &from, &to, &weight) != 3) return 0;
        
        if (from < 0 || from >= node_count || to < 0 || to >= node_count) {
            printf("Invalid node indices\n");
            return 0;
        }
        
        if (graph->edge_count >= MAX_EDGES) return 0;
        
        graph->edges[graph->edge_count].from = from;
        graph->edges[graph->edge_count].to = to;
        graph->edges[graph->edge_count].weight = weight;
        graph->edge_count++;
    }
    
    return 1;
}

void print_graph(const Graph *graph) {
    printf("Graph with %d nodes and %d edges:\n", graph->node_count, graph->edge_count);
    for (int i = 0; i < graph->edge_count; i++) {
        printf("  %d -> %d (weight: %d)\n", 
               graph->edges[i].from, 
               graph->edges[i].to, 
               graph->edges[i].weight);
    }
}

int compare_edges(const void *a, const void *b) {
    const Edge *edge1 = (const Edge *)a;
    const Edge *edge2 = (const Edge *)b;
    if (edge1->weight < edge2->weight) return -1;
    if (edge1->weight > edge2->weight) return 1;
    return 0;
}

int find_parent(int parent[], int node) {
    if (parent[node] != node) {
        parent[node] = find_parent(parent, parent[node]);
    }
    return parent[node];
}

int union_sets(int parent[], int rank[], int x, int y) {
    int root_x = find_parent(parent, x);
    int root_y = find_parent(parent, y);
    
    if (root_x == root_y) return 0;
    
    if (rank[root_x] < rank[root_y]) {
        parent[root_x] = root_y;
    } else if (rank[root_x] > rank[root_y]) {
        parent[root_y] = root_x;
    } else {
        parent[root_y] = root_x;
        rank[root_x]++;
    }
    return 1;
}

void kruskal_mst(const Graph *graph) {
    if (graph->node_count == 0 || graph->edge_count == 0) {
        printf("Graph is empty\n");
        return;
    }
    
    Edge sorted_edges[MAX_EDGES];
    memcpy(sorted_edges, graph->edges, graph->edge_count * sizeof(Edge));
    qsort(sorted_edges, graph->edge_count, sizeof(Edge), compare_edges);
    
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    for (int i = 0; i < graph->node_count; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
    
    Edge mst_edges[MAX_NODES - 1];
    int mst_count = 0;
    int total_weight = 0;
    
    for (int i = 0; i < graph->edge_count && mst_count < graph->node_count - 1; i++) {
        if (union_sets(parent, rank, sorted_edges[i].from, sorted_edges[i].to)) {
            mst_edges[mst_count] = sorted_edges[i];
            total_weight += sorted_edges[i].weight;
            mst_count++;
        }
    }
    
    if (mst_count != graph->node_count - 1) {
        printf("Graph is not connected\n");
        return;
    }
    
    printf("Minimum Spanning Tree (Kruskal's algorithm):\n");
    printf("Total weight: %d\n", total_weight);
    for (int i = 0; i < mst