//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

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
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &graph->node_count) != 1) {
        return 0;
    }
    if (graph->node_count < 1 || graph->node_count > MAX_NODES) {
        return 0;
    }

    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &graph->edge_count) != 1) {
        return 0;
    }
    if (graph->edge_count < 0 || graph->edge_count > MAX_EDGES) {
        return 0;
    }

    printf("Enter edges as 'from to weight' (nodes 0-%d):\n", graph->node_count - 1);
    for (int i = 0; i < graph->edge_count; i++) {
        if (scanf("%d %d %d", &graph->edges[i].from, &graph->edges[i].to, &graph->edges[i].weight) != 3) {
            return 0;
        }
        if (graph->edges[i].from < 0 || graph->edges[i].from >= graph->node_count ||
            graph->edges[i].to < 0 || graph->edges[i].to >= graph->node_count) {
            return 0;
        }
    }
    return 1;
}

int compare_edges(const void *a, const void *b) {
    const Edge *edge_a = (const Edge *)a;
    const Edge *edge_b = (const Edge *)b;
    if (edge_a->weight < edge_b->weight) return -1;
    if (edge_a->weight > edge_b->weight) return 1;
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
    
    if (root_x == root_y) {
        return 0;
    }
    
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

void kruskal_mst(Graph *graph) {
    if (graph->node_count == 0) {
        printf("Graph is empty\n");
        return;
    }

    Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    
    for (int i = 0; i < graph->node_count; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
    
    Edge edges_copy[MAX_EDGES];
    memcpy(edges_copy, graph->edges, graph->edge_count * sizeof(Edge));
    qsort(edges_copy, graph->edge_count, sizeof(Edge), compare_edges);
    
    int result_count = 0;
    int total_weight = 0;
    
    for (int i = 0; i < graph->edge_count && result_count < graph->node_count - 1; i++) {
        Edge current_edge = edges_copy[i];
        
        if (union_sets(parent, rank, current_edge.from, current_edge.to)) {
            result[result_count] = current_edge;
            total_weight += current_edge.weight;
            result_count++;
        }
    }
    
    if (result_count != graph->node_count - 1) {
        printf("Graph is not connected\n");
        return;
    }
    
    printf("Minimum Spanning Tree edges:\n");
    for (int i = 0; i < result_count; i++) {
        printf("%d - %d (weight: %d)\n", result[i].from, result[i].to, result[i].weight);
    }
    printf("Total weight: %d\n", total_weight);
}

int main() {
    Graph graph;
    
    printf("Graph Minimum Spanning Tree (Kruskal's Algorithm)\n");
    printf("=================================================\n");
    
    if (!read_graph(&graph)) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    kruskal_mst(&graph);
    
    return 0;
}