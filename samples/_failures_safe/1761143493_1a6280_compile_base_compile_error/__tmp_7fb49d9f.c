//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
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

int validate_node(int node, int max_nodes) {
    return node >= 0 && node < max_nodes;
}

int validate_edge(struct Edge *edge, int max_nodes) {
    if (!validate_node(edge->src, max_nodes)) return 0;
    if (!validate_node(edge->dest, max_nodes)) return 0;
    if (edge->weight < 0) return 0;
    return 1;
}

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph->num_edges >= MAX_EDGES) return 0;
    if (!validate_node(src, graph->num_nodes)) return 0;
    if (!validate_node(dest, graph->num_nodes)) return 0;
    if (weight < 0) return 0;
    
    struct Edge *edge = &graph->edges[graph->num_edges];
    edge->src = src;
    edge->dest = dest;
    edge->weight = weight;
    graph->num_edges++;
    return 1;
}

void print_graph(struct Graph *graph) {
    printf("Graph with %d nodes and %d edges:\n", graph->num_nodes, graph->num_edges);
    for (int i = 0; i < graph->num_edges; i++) {
        struct Edge *edge = &graph->edges[i];
        printf("  %d -> %d (weight: %d)\n", edge->src, edge->dest, edge->weight);
    }
}

int find(int parent[], int i) {
    if (parent[i] != i) {
        parent[i] = find(parent, parent[i]);
    }
    return parent[i];
}

void union_sets(int parent[], int rank[], int x, int y) {
    int xroot = find(parent, x);
    int yroot = find(parent, y);
    
    if (rank[xroot] < rank[yroot]) {
        parent[xroot] = yroot;
    } else if (rank[xroot] > rank[yroot]) {
        parent[yroot] = xroot;
    } else {
        parent[yroot] = xroot;
        rank[xroot]++;
    }
}

int compare_edges(const void *a, const void *b) {
    struct Edge *edge1 = (struct Edge *)a;
    struct Edge *edge2 = (struct Edge *)b;
    if (edge1->weight < edge2->weight) return -1;
    if (edge1->weight > edge2->weight) return 1;
    return 0;
}

void kruskal_mst(struct Graph *graph) {
    if (graph->num_nodes == 0 || graph->num_edges == 0) {
        printf("Graph is empty\n");
        return;
    }
    
    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    int e = 0;
    int i = 0;
    
    for (int v = 0; v < graph->num_nodes; v++) {
        parent[v] = v;
        rank[v] = 0;
    }
    
    struct Edge sorted_edges[MAX_EDGES];
    memcpy(sorted_edges, graph->edges, graph->num_edges * sizeof(struct Edge));
    qsort(sorted_edges, graph->num_edges, sizeof(struct Edge), compare_edges);
    
    while (e < graph->num_nodes - 1 && i < graph->num_edges) {
        struct Edge next_edge = sorted_edges[i++];
        
        int x = find(parent, next_edge.src);
        int y = find(parent, next_edge.dest);
        
        if (x != y) {
            result[e++] = next_edge;
            union_sets(parent, rank, x, y);
        }
    }
    
    printf("Minimum Spanning Tree (Kruskal's algorithm):\n");
    int total_weight = 0;
    for (int j = 0; j < e; j++) {
        printf("  %d -> %d (weight: %d)\n", result[j].src, result[j].dest, result[j].weight);
        total_weight += result[j].weight;
    }
    printf("Total weight: %d\n", total_weight);
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Enter number of nodes (max %d): ", MAX_NODES);
    int num_nodes;
    if (scanf("%d", &num_nodes) != 1 || num_nodes <= 0 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    graph.num_nodes = num_nodes;
    
    printf("Enter number of edges (max %d): ", MAX_EDGES);
    int num_edges;
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as: source destination weight