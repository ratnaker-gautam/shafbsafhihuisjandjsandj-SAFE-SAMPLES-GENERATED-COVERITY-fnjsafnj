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

int add_node(struct Graph *graph) {
    if (graph->num_nodes >= MAX_NODES) {
        return -1;
    }
    return graph->num_nodes++;
}

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph->num_edges >= MAX_EDGES) {
        return 0;
    }
    if (src < 0 || src >= graph->num_nodes || dest < 0 || dest >= graph->num_nodes) {
        return 0;
    }
    if (src == dest) {
        return 0;
    }
    graph->edges[graph->num_edges].src = src;
    graph->edges[graph->num_edges].dest = dest;
    graph->edges[graph->num_edges].weight = weight;
    graph->num_edges++;
    return 1;
}

void print_graph(struct Graph *graph) {
    printf("Graph has %d nodes and %d edges:\n", graph->num_nodes, graph->num_edges);
    for (int i = 0; i < graph->num_edges; i++) {
        printf("Edge %d: %d -> %d (weight %d)\n", i, graph->edges[i].src, graph->edges[i].dest, graph->edges[i].weight);
    }
}

int compare_edges(const void *a, const void *b) {
    struct Edge *edgeA = (struct Edge *)a;
    struct Edge *edgeB = (struct Edge *)b;
    if (edgeA->weight < edgeB->weight) return -1;
    if (edgeA->weight > edgeB->weight) return 1;
    return 0;
}

int find_parent(int parent[], int node) {
    if (parent[node] != node) {
        parent[node] = find_parent(parent, parent[node]);
    }
    return parent[node];
}

void union_sets(int parent[], int rank[], int x, int y) {
    int rootX = find_parent(parent, x);
    int rootY = find_parent(parent, y);
    if (rootX != rootY) {
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }
}

void kruskal_mst(struct Graph *graph) {
    if (graph->num_nodes == 0) {
        printf("Graph is empty.\n");
        return;
    }
    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    for (int i = 0; i < graph->num_nodes; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
    qsort(graph->edges, graph->num_edges, sizeof(struct Edge), compare_edges);
    int edge_count = 0;
    int result_index = 0;
    while (edge_count < graph->num_nodes - 1 && result_index < graph->num_edges) {
        struct Edge next_edge = graph->edges[result_index++];
        int x = find_parent(parent, next_edge.src);
        int y = find_parent(parent, next_edge.dest);
        if (x != y) {
            result[edge_count++] = next_edge;
            union_sets(parent, rank, x, y);
        }
    }
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int i = 0; i < edge_count; i++) {
        printf("%d - %d (weight %d)\n", result[i].src, result[i].dest, result[i].weight);
        total_weight += result[i].weight;
    }
    printf("Total MST weight: %d\n", total_weight);
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    printf("Graph Construction:\n");
    printf("Adding nodes...\n");
    int node_count;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    if (node_count < 1 || node_count > MAX_NODES) {
        printf("Invalid number of nodes.\n");
        return 1;
    }
    for (int i = 0; i < node_count; i++) {
        add_node(&graph);
    }
    printf("Added %d nodes.\n", node_count);
    printf("Adding edges...\n");
    int edge_count;
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &edge_count) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    if (edge_count < 0 || edge_count > MAX_EDGES) {
        printf("Invalid number