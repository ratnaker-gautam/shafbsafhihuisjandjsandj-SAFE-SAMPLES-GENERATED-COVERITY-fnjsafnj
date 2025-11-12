//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: graph_algorithms
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
    int i = 0;
    
    for (int node = 0; node < graph->num_nodes; node++) {
        parent[node] = node;
        rank[node] = 0;
    }
    
    struct Edge sorted_edges[MAX_EDGES];
    for (int j = 0; j < graph->num_edges; j++) {
        sorted_edges[j] = graph->edges[j];
    }
    qsort(sorted_edges, graph->num_edges, sizeof(struct Edge), compare_edges);
    
    while (edge_count < graph->num_nodes - 1 && i < graph->num_edges) {
        struct Edge next_edge = sorted_edges[i++];
        
        int x = find_parent(parent, next_edge.src);
        int y = find_parent(parent, next_edge.dest);
        
        if (x != y) {
            result[edge_count++] = next_edge;
            union_sets(parent, rank, x, y);
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int j = 0; j < edge_count; j++) {
        printf("%d - %d : %d\n", result[j].src, result[j].dest, result[j].weight);
        total_weight += result[j].weight;
    }
    printf("Total weight: %d\n", total_weight);
}

void dijkstra_shortest_path(struct Graph* graph, int start_node) {
    if (!validate_node(graph, start_node)) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->num_nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[start_node] = 0;
    
    for (int count = 0; count < graph->num_nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int v = 0; v < graph->num_nodes; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_node = v;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int i = 0; i < graph->num_edges; i++) {
            if (graph->edges[i].src == min_node) {
                int neighbor =