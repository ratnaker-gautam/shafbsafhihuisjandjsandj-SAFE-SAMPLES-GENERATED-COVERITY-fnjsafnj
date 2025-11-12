//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
    if (graph == NULL) return;
    graph->num_nodes = 0;
    graph->num_edges = 0;
}

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph == NULL) return 0;
    if (src < 0 || dest < 0) return 0;
    if (graph->num_edges >= MAX_EDGES) return 0;
    if (weight < 0) return 0;
    
    graph->edges[graph->num_edges].src = src;
    graph->edges[graph->num_edges].dest = dest;
    graph->edges[graph->num_edges].weight = weight;
    graph->num_edges++;
    return 1;
}

int compare_edges(const void *a, const void *b) {
    const struct Edge *edge1 = (const struct Edge *)a;
    const struct Edge *edge2 = (const struct Edge *)b;
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

void kruskal_mst(struct Graph *graph) {
    if (graph == NULL) return;
    if (graph->num_nodes <= 0 || graph->num_edges <= 0) return;
    
    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    int e = 0;
    int i = 0;
    
    for (int v = 0; v < graph->num_nodes; v++) {
        parent[v] = v;
        rank[v] = 0;
    }
    
    qsort(graph->edges, graph->num_edges, sizeof(struct Edge), compare_edges);
    
    while (e < graph->num_nodes - 1 && i < graph->num_edges) {
        struct Edge next_edge = graph->edges[i++];
        
        int x = find_parent(parent, next_edge.src);
        int y = find_parent(parent, next_edge.dest);
        
        if (x != y) {
            result[e++] = next_edge;
            union_sets(parent, rank, x, y);
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int j = 0; j < e; j++) {
        printf("%d -- %d : %d\n", result[j].src, result[j].dest, result[j].weight);
        total_weight += result[j].weight;
    }
    printf("Total weight: %d\n", total_weight);
}

void dijkstra_shortest_path(struct Graph *graph, int start_node) {
    if (graph == NULL) return;
    if (start_node < 0 || start_node >= graph->num_nodes) return;
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->num_nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[start_node] = 0;
    
    for (int count = 0; count < graph->num_nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_index = -1;
        
        for (int v = 0; v < graph->num_nodes; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_index = v;
            }
        }
        
        if (min_index == -1) break;
        visited[min_index] = 1;
        
        for (int i = 0; i < graph->num_edges; i++) {
            if (graph->edges[i].src == min_index) {
                int v = graph->edges[i].dest;
                int weight = graph->edges[i].weight;
                if (!visited[v] && dist[min_index] != INT_MAX && 
                    dist[min_index] + weight < dist[v]) {
                    dist[v] = dist[min_index] + weight;
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", start_node);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (dist[i] == INT