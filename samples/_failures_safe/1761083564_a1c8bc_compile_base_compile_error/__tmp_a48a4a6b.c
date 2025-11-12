//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
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

void initialize_graph(struct Graph *graph, int nodes, int edges) {
    if (graph == NULL || nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        return;
    }
    graph->num_nodes = nodes;
    graph->num_edges = edges;
    for (int i = 0; i < edges; i++) {
        (graph->edges + i)->src = -1;
        (graph->edges + i)->dest = -1;
        (graph->edges + i)->weight = 0;
    }
}

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph == NULL || src < 0 || src >= graph->num_nodes || dest < 0 || dest >= graph->num_nodes || weight < 0) {
        return 0;
    }
    if (graph->num_edges >= MAX_EDGES) {
        return 0;
    }
    struct Edge *edge_ptr = graph->edges + graph->num_edges;
    edge_ptr->src = src;
    edge_ptr->dest = dest;
    edge_ptr->weight = weight;
    graph->num_edges++;
    return 1;
}

void print_graph(struct Graph *graph) {
    if (graph == NULL) {
        return;
    }
    printf("Graph with %d nodes and %d edges:\n", graph->num_nodes, graph->num_edges);
    for (int i = 0; i < graph->num_edges; i++) {
        struct Edge *edge_ptr = graph->edges + i;
        printf("Edge %d: %d -> %d (weight %d)\n", i, edge_ptr->src, edge_ptr->dest, edge_ptr->weight);
    }
}

void dfs_util(struct Graph *graph, int node, int *visited) {
    if (graph == NULL || visited == NULL || node < 0 || node >= graph->num_nodes) {
        return;
    }
    *(visited + node) = 1;
    printf("%d ", node);
    for (int i = 0; i < graph->num_edges; i++) {
        struct Edge *edge_ptr = graph->edges + i;
        if (edge_ptr->src == node && *(visited + edge_ptr->dest) == 0) {
            dfs_util(graph, edge_ptr->dest, visited);
        }
    }
}

void dfs(struct Graph *graph, int start) {
    if (graph == NULL || start < 0 || start >= graph->num_nodes) {
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < graph->num_nodes; i++) {
        *(visited + i) = 0;
    }
    printf("DFS traversal from node %d: ", start);
    dfs_util(graph, start, visited);
    printf("\n");
}

int compare_edges(const void *a, const void *b) {
    const struct Edge *edge_a = (const struct Edge *)a;
    const struct Edge *edge_b = (const struct Edge *)b;
    if (edge_a->weight < edge_b->weight) return -1;
    if (edge_a->weight > edge_b->weight) return 1;
    return 0;
}

int find_parent(int *parent, int node) {
    if (parent == NULL) {
        return -1;
    }
    if (*(parent + node) != node) {
        *(parent + node) = find_parent(parent, *(parent + node));
    }
    return *(parent + node);
}

void union_sets(int *parent, int *rank, int x, int y) {
    if (parent == NULL || rank == NULL) {
        return;
    }
    int root_x = find_parent(parent, x);
    int root_y = find_parent(parent, y);
    if (root_x == root_y) {
        return;
    }
    if (*(rank + root_x) < *(rank + root_y)) {
        *(parent + root_x) = root_y;
    } else if (*(rank + root_x) > *(rank + root_y)) {
        *(parent + root_y) = root_x;
    } else {
        *(parent + root_y) = root_x;
        *(rank + root_x) = *(rank + root_x) + 1;
    }
}

void kruskal_mst(struct Graph *graph) {
    if (graph == NULL || graph->num_nodes <= 0 || graph->num_edges <= 0) {
        return;
    }
    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    for (int i = 0; i < graph->num_nodes; i++) {
        *(parent + i) = i;
        *(rank + i) = 0;
    }
    qsort(graph->edges, graph->num_edges, sizeof(struct Edge), compare_edges);
    int result_count = 0;
    int edge_index = 0;
    while (result_count < graph->num_nodes - 1 && edge_index < graph->num_edges) {
        struct Edge *next_edge = graph->edges + edge_index;