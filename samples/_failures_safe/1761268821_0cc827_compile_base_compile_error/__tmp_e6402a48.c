//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000
#define MAX_LINE 1024

typedef struct {
    int from;
    int to;
    int weight;
} Edge;

typedef struct {
    int node_count;
    int edge_count;
    Edge edges[MAX_EDGES];
} Graph;

int read_graph(Graph *graph) {
    char buffer[MAX_LINE];
    int node_count, edge_count;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    if (sscanf(buffer, "%d", &node_count) != 1) return 0;
    if (node_count < 1 || node_count > MAX_NODES) return 0;
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    if (sscanf(buffer, "%d", &edge_count) != 1) return 0;
    if (edge_count < 0 || edge_count > MAX_EDGES) return 0;
    
    graph->node_count = node_count;
    graph->edge_count = 0;
    
    for (int i = 0; i < edge_count; i++) {
        int from, to, weight;
        printf("Enter edge %d (from to weight): ", i + 1);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
        if (sscanf(buffer, "%d %d %d", &from, &to, &weight) != 3) return 0;
        
        if (from < 0 || from >= node_count || to < 0 || to >= node_count) {
            printf("Invalid node indices\n");
            return 0;
        }
        
        if (graph->edge_count >= MAX_EDGES) return 0;
        
        graph->edges[graph->edge_count].from = from;
        graph->edges[graph->edge_count].to = to;
        graph->edges[graph->edge_count].weight = weight;
        graph->edge_count++;
    }
    
    return 1;
}

void print_graph(const Graph *graph) {
    printf("Graph with %d nodes and %d edges:\n", graph->node_count, graph->edge_count);
    for (int i = 0; i < graph->edge_count; i++) {
        printf("  %d -> %d (weight: %d)\n", 
               graph->edges[i].from, graph->edges[i].to, graph->edges[i].weight);
    }
}

int compare_edges(const void *a, const void *b) {
    const Edge *edge1 = (const Edge *)a;
    const Edge *edge2 = (const Edge *)b;
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

int kruskal_mst(const Graph *graph, Edge mst[]) {
    if (graph->node_count == 0) return 0;
    
    Edge sorted_edges[MAX_EDGES];
    memcpy(sorted_edges, graph->edges, graph->edge_count * sizeof(Edge));
    qsort(sorted_edges, graph->edge_count, sizeof(Edge), compare_edges);
    
    int parent[MAX_NODES];
    for (int i = 0; i < graph->node_count; i++) {
        parent[i] = i;
    }
    
    int mst_edges = 0;
    int mst_weight = 0;
    
    for (int i = 0; i < graph->edge_count && mst_edges < graph->node_count - 1; i++) {
        int u = sorted_edges[i].from;
        int v = sorted_edges[i].to;
        int weight = sorted_edges[i].weight;
        
        int root_u = find_parent(parent, u);
        int root_v = find_parent(parent, v);
        
        if (root_u != root_v) {
            mst[mst_edges].from = u;
            mst[mst_edges].to = v;
            mst[mst_edges].weight = weight;
            mst_edges++;
            mst_weight += weight;
            parent[root_u] = root_v;
        }
    }
    
    if (mst_edges != graph->node_count - 1) {
        printf("Graph is not connected\n");
        return -1;
    }
    
    return mst_weight;
}

int main() {
    Graph graph;
    Edge mst[MAX_NODES];
    
    printf("Graph Minimum Spanning Tree (Kruskal's Algorithm)\n");
    printf("=================================================\n");
    
    if (!read_graph(&graph)) {
        printf("Error reading graph data\n");
        return 1;
    }
    
    print_graph(&graph);
    
    int mst_weight = kruskal_mst(&graph, mst);
    
    if (mst_weight >= 0) {
        printf("\nMinimum Spanning Tree (total weight: %d):\n", mst_weight);
        for (int i =