//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
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

void initialize_graph(struct Graph* graph, int nodes, int edges) {
    if (nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    graph->num_nodes = nodes;
    graph->num_edges = edges;
}

int validate_node(struct Graph* graph, int node) {
    return node >= 0 && node < graph->num_nodes;
}

void add_edge(struct Graph* graph, int edge_index, int src, int dest, int weight) {
    if (edge_index < 0 || edge_index >= graph->num_edges) {
        fprintf(stderr, "Invalid edge index\n");
        exit(1);
    }
    if (!validate_node(graph, src) || !validate_node(graph, dest)) {
        fprintf(stderr, "Invalid node in edge\n");
        exit(1);
    }
    graph->edges[edge_index].src = src;
    graph->edges[edge_index].dest = dest;
    graph->edges[edge_index].weight = weight;
}

int compare_edges(const void* a, const void* b) {
    const struct Edge* edge1 = (const struct Edge*)a;
    const struct Edge* edge2 = (const struct Edge*)b;
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

void union_sets(int parent[], int rank[], int x, int y) {
    int root_x = find_parent(parent, x);
    int root_y = find_parent(parent, y);
    
    if (root_x != root_y) {
        if (rank[root_x] < rank[root_y]) {
            parent[root_x] = root_y;
        } else if (rank[root_x] > rank[root_y]) {
            parent[root_y] = root_x;
        } else {
            parent[root_y] = root_x;
            rank[root_x]++;
        }
    }
}

void kruskal_mst(struct Graph* graph) {
    if (graph->num_nodes <= 0 || graph->num_edges < 0) {
        fprintf(stderr, "Invalid graph for MST\n");
        return;
    }
    
    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    int edge_count = 0;
    
    for (int i = 0; i < graph->num_nodes; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
    
    struct Edge sorted_edges[MAX_EDGES];
    for (int i = 0; i < graph->num_edges; i++) {
        sorted_edges[i] = graph->edges[i];
    }
    qsort(sorted_edges, graph->num_edges, sizeof(struct Edge), compare_edges);
    
    for (int i = 0; i < graph->num_edges && edge_count < graph->num_nodes - 1; i++) {
        struct Edge current_edge = sorted_edges[i];
        int src_parent = find_parent(parent, current_edge.src);
        int dest_parent = find_parent(parent, current_edge.dest);
        
        if (src_parent != dest_parent) {
            result[edge_count] = current_edge;
            edge_count++;
            union_sets(parent, rank, src_parent, dest_parent);
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int i = 0; i < edge_count; i++) {
        printf("%d - %d : %d\n", result[i].src, result[i].dest, result[i].weight);
        total_weight += result[i].weight;
    }
    printf("Total weight: %d\n", total_weight);
}

int main() {
    struct Graph graph;
    int num_nodes, num_edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1) {
        fprintf(stderr, "Invalid input for nodes\n");
        return 1;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &num_edges) != 1) {
        fprintf(stderr, "Invalid input for edges\n");
        return 1;
    }
    
    initialize_graph(&graph, num_nodes, num_edges);
    
    printf("Enter edges as: source destination weight\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&graph, i, src, dest, weight);
    }
    
    kruskal_mst(&graph);