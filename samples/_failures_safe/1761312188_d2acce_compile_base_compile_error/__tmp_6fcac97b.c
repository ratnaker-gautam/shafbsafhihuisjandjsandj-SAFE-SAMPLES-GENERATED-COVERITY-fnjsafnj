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

void initialize_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
}

int add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g->num_edges >= MAX_EDGES) return 0;
    if (src < 0 || dest < 0 || src >= MAX_NODES || dest >= MAX_NODES) return 0;
    if (weight < 0) return 0;
    
    g->edges[g->num_edges].src = src;
    g->edges[g->num_edges].dest = dest;
    g->edges[g->num_edges].weight = weight;
    g->num_edges++;
    if (src >= g->num_nodes) g->num_nodes = src + 1;
    if (dest >= g->num_nodes) g->num_nodes = dest + 1;
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
    struct Edge *edge1 = (struct Edge *)a;
    struct Edge *edge2 = (struct Edge *)b;
    if (edge1->weight > edge2->weight) return 1;
    if (edge1->weight < edge2->weight) return -1;
    return 0;
}

void kruskal_mst(struct Graph *g) {
    if (g->num_nodes == 0 || g->num_edges == 0) {
        printf("Graph is empty\n");
        return;
    }
    
    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    int e = 0;
    int i = 0;
    
    for (int v = 0; v < g->num_nodes; v++) {
        parent[v] = v;
        rank[v] = 0;
    }
    
    qsort(g->edges, g->num_edges, sizeof(g->edges[0]), compare_edges);
    
    while (e < g->num_nodes - 1 && i < g->num_edges) {
        struct Edge next_edge = g->edges[i++];
        
        int x = find(parent, next_edge.src);
        int y = find(parent, next_edge.dest);
        
        if (x != y) {
            result[e++] = next_edge;
            union_sets(parent, rank, x, y);
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (i = 0; i < e; i++) {
        printf("%d -- %d (weight: %d)\n", result[i].src, result[i].dest, result[i].weight);
        total_weight += result[i].weight;
    }
    printf("Total weight: %d\n", total_weight);
}

void dijkstra(struct Graph *g, int src) {
    if (g->num_nodes == 0) {
        printf("Graph is empty\n");
        return;
    }
    if (src < 0 || src >= g->num_nodes) {
        printf("Invalid source node\n");
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[src] = 0;
    
    for (int count = 0; count < g->num_nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_index = -1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_index = v;
            }
        }
        
        if (min_index == -1) break;
        
        visited[min_index] = 1;
        
        for (int i = 0; i < g->num_edges; i++) {
            if (g->edges[i].src == min_index) {
                int v = g->edges[i].dest;
                int weight = g->edges[i].weight;
                if (!visited[v] && dist[min_index] != INT_MAX && 
                    dist[min_index] + weight < dist[v]) {
                    dist[v] = dist[min_index] + weight;
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", src