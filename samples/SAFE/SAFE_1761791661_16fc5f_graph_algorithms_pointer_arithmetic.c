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

void initialize_graph(struct Graph *graph) {
    graph->num_nodes = 0;
    graph->num_edges = 0;
}

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph->num_edges >= MAX_EDGES) return 0;
    if (src < 0 || dest < 0 || src >= MAX_NODES || dest >= MAX_NODES) return 0;
    
    struct Edge *edge_ptr = graph->edges + graph->num_edges;
    edge_ptr->src = src;
    edge_ptr->dest = dest;
    edge_ptr->weight = weight;
    graph->num_edges++;
    
    if (src >= graph->num_nodes) graph->num_nodes = src + 1;
    if (dest >= graph->num_nodes) graph->num_nodes = dest + 1;
    
    return 1;
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

void kruskal_mst(struct Graph *graph) {
    if (graph->num_nodes == 0 || graph->num_edges == 0) {
        printf("Graph is empty\n");
        return;
    }
    
    struct Edge result[MAX_NODES];
    int result_count = 0;
    
    qsort(graph->edges, graph->num_edges, sizeof(struct Edge), compare_edges);
    
    int parent[MAX_NODES];
    for (int i = 0; i < graph->num_nodes; i++) {
        parent[i] = i;
    }
    
    struct Edge *edge_ptr = graph->edges;
    for (int i = 0; i < graph->num_edges; i++) {
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
        printf("%d - %d : %d\n", result_ptr->src, result_ptr->dest, result_ptr->weight);
        total_weight += result_ptr->weight;
        result_ptr++;
    }
    printf("Total weight: %d\n", total_weight);
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Enter number of edges: ");
    int num_edges;
    if (scanf("%d", &num_edges) != 1 || num_edges <= 0 || num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (src dest weight):\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid input\n");
            return 1;
        }
        if (!add_edge(&graph, src, dest, weight)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    kruskal_mst(&graph);
    
    return 0;
}