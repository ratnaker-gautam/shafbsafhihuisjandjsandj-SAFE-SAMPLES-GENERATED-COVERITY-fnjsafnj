//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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
    if (src < 0 || src >= MAX_NODES) return 0;
    if (dest < 0 || dest >= MAX_NODES) return 0;
    if (weight < 0) return 0;
    
    graph->edges[graph->num_edges].src = src;
    graph->edges[graph->num_edges].dest = dest;
    graph->edges[graph->num_edges].weight = weight;
    graph->num_edges++;
    return 1;
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
    struct Edge *edge1 = (struct Edge*)a;
    struct Edge *edge2 = (struct Edge*)b;
    return edge1->weight - edge2->weight;
}

int kruskal_mst(struct Graph *graph, struct Edge result[]) {
    if (graph->num_nodes <= 0 || graph->num_edges <= 0) return 0;
    
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    
    for (int i = 0; i < graph->num_nodes; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
    
    struct Edge edges_copy[MAX_EDGES];
    memcpy(edges_copy, graph->edges, graph->num_edges * sizeof(struct Edge));
    
    qsort(edges_copy, graph->num_edges, sizeof(struct Edge), compare_edges);
    
    int e = 0;
    int i = 0;
    
    while (e < graph->num_nodes - 1 && i < graph->num_edges) {
        struct Edge next_edge = edges_copy[i++];
        
        int x = find(parent, next_edge.src);
        int y = find(parent, next_edge.dest);
        
        if (x != y) {
            result[e++] = next_edge;
            union_sets(parent, rank, x, y);
        }
    }
    
    return e;
}

void print_mst(struct Edge result[], int num_edges) {
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int i = 0; i < num_edges; i++) {
        printf("%d - %d : %d\n", result[i].src, result[i].dest, result[i].weight);
        total_weight += result[i].weight;
    }
    printf("Total weight: %d\n", total_weight);
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Enter number of nodes (max %d): ", MAX_NODES);
    if (scanf("%d", &graph.num_nodes) != 1 || graph.num_nodes <= 0 || graph.num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges (max %d): ", MAX_EDGES);
    if (scanf("%d", &graph.num_edges) != 1 || graph.num_edges <= 0 || graph.num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as: source destination weight\n");
    for (int i = 0; i < graph.num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        
        if (!add_edge(&graph, src, dest, weight)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    struct Edge mst_result[MAX_NODES - 1];
    int mst_edges = kruskal_mst(&graph, mst_result);
    
    if (mst_edges == graph.num_nodes - 1) {
        print_mst(mst_result, mst_edges);
    } else {
        printf("No spanning tree exists\n");
    }
    
    return 0;
}