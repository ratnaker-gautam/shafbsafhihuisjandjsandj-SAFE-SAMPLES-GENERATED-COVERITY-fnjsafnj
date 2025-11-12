//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
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

void initialize_graph(struct Graph* graph) {
    graph->num_nodes = 0;
    graph->num_edges = 0;
}

int add_edge(struct Graph* graph, int src, int dest, int weight) {
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

int compare_edges(const void* a, const void* b) {
    struct Edge* edge_a = (struct Edge*)a;
    struct Edge* edge_b = (struct Edge*)b;
    if (edge_a->weight < edge_b->weight) return -1;
    if (edge_a->weight > edge_b->weight) return 1;
    return 0;
}

void kruskal_mst(struct Graph* graph) {
    if (graph->num_nodes == 0 || graph->num_edges == 0) {
        printf("Graph is empty\n");
        return;
    }
    
    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    int result_count = 0;
    
    for (int i = 0; i < graph->num_nodes; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
    
    qsort(graph->edges, graph->num_edges, sizeof(struct Edge), compare_edges);
    
    for (int i = 0; i < graph->num_edges && result_count < graph->num_nodes - 1; i++) {
        struct Edge current_edge = graph->edges[i];
        
        if (current_edge.src >= graph->num_nodes || current_edge.dest >= graph->num_nodes) {
            continue;
        }
        
        int src_root = find_parent(parent, current_edge.src);
        int dest_root = find_parent(parent, current_edge.dest);
        
        if (src_root != dest_root) {
            result[result_count] = current_edge;
            result_count++;
            union_sets(parent, rank, src_root, dest_root);
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int i = 0; i < result_count; i++) {
        printf("%d - %d : %d\n", result[i].src, result[i].dest, result[i].weight);
        total_weight += result[i].weight;
    }
    printf("Total weight: %d\n", total_weight);
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Enter number of nodes (max %d): ", MAX_NODES);
    int num_nodes;
    if (scanf("%d", &num_nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (num_nodes <= 0 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    graph.num_nodes = num_nodes;
    
    printf("Enter number of edges: ");
    int num_edges;
    if (scanf("%d", &num_edges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (num_edges <= 0 || num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as: source destination weight\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        
        if (src < 0 || src >= num_nodes || dest < 0 || dest >= num_nodes) {
            printf("Invalid node indices\n");
            return 1;
        }
        
        if (weight < 0) {
            printf("Negative weights not