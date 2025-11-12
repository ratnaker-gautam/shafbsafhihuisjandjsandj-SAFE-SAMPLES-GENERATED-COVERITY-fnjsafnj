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
    struct Edge *edgeA = (struct Edge *)a;
    struct Edge *edgeB = (struct Edge *)b;
    if (edgeA->weight < edgeB->weight) return -1;
    if (edgeA->weight > edgeB->weight) return 1;
    return 0;
}

void kruskal_mst(struct Graph *graph) {
    if (graph->num_nodes == 0) {
        printf("Graph is empty\n");
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
    int e = 0;
    int i = 0;
    while (e < graph->num_nodes - 1 && i < graph->num_edges) {
        struct Edge next_edge = graph->edges[i++];
        int x = find(parent, next_edge.src);
        int y = find(parent, next_edge.dest);
        if (x != y) {
            result[e++] = next_edge;
            union_sets(parent, rank, x, y);
        }
    }
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int j = 0; j < e; j++) {
        printf("%d -- %d (weight %d)\n", result[j].src, result[j].dest, result[j].weight);
        total_weight += result[j].weight;
    }
    printf("Total MST weight: %d\n", total_weight);
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    printf("Graph Construction\n");
    printf("Enter number of nodes (max %d): ", MAX_NODES);
    int num_nodes;
    if (scanf("%d", &num_nodes) != 1 || num_nodes <= 0 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    for (int i = 0; i < num_nodes; i++) {
        add_node(&graph);
    }
    printf("Enter number of edges (max %d): ", MAX_EDGES);
    int num_edges;
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        printf("Enter edge %d (src dest weight): ", i);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!