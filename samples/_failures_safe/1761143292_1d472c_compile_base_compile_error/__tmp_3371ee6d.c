//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int num_nodes;
    int num_edges;
    int adjacency[MAX_NODES][MAX_NODES];
    int edge_weights[MAX_NODES][MAX_NODES];
};

struct Edge {
    int u;
    int v;
    int weight;
};

void initialize_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g->adjacency[i][j] = 0;
            g->edge_weights[i][j] = 0;
        }
    }
}

int add_edge(struct Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= MAX_NODES || v < 0 || v >= MAX_NODES) {
        return -1;
    }
    if (u == v) {
        return -1;
    }
    if (g->adjacency[u][v] != 0) {
        return -1;
    }
    if (weight < 0) {
        return -1;
    }
    
    g->adjacency[u][v] = 1;
    g->adjacency[v][u] = 1;
    g->edge_weights[u][v] = weight;
    g->edge_weights[v][u] = weight;
    g->num_edges++;
    return 0;
}

int add_node(struct Graph *g) {
    if (g->num_nodes >= MAX_NODES) {
        return -1;
    }
    g->num_nodes++;
    return g->num_nodes - 1;
}

void dfs_util(struct Graph *g, int node, int visited[]) {
    if (node < 0 || node >= g->num_nodes) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adjacency[node][i] && !visited[i]) {
            dfs_util(g, i, visited);
        }
    }
}

void depth_first_search(struct Graph *g, int start_node) {
    if (start_node < 0 || start_node >= g->num_nodes) {
        printf("Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from node %d: ", start_node);
    dfs_util(g, start_node, visited);
    printf("\n");
}

void breadth_first_search(struct Graph *g, int start_node) {
    if (start_node < 0 || start_node >= g->num_nodes) {
        printf("Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start_node] = 1;
    queue[rear++] = start_node;
    
    printf("BFS traversal from node %d: ", start_node);
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adjacency[current][i] && !visited[i]) {
                visited[i] = 1;
                queue[rear++] = i;
            }
        }
    }
    printf("\n");
}

int compare_edges(const void *a, const void *b) {
    struct Edge *edgeA = (struct Edge *)a;
    struct Edge *edgeB = (struct Edge *)b;
    return edgeA->weight - edgeB->weight;
}

int find_parent(int parent[], int node) {
    if (parent[node] != node) {
        parent[node] = find_parent(parent, parent[node]);
    }
    return parent[node];
}

void kruskal_mst(struct Graph *g) {
    if (g->num_nodes == 0) {
        printf("Graph is empty\n");
        return;
    }
    
    struct Edge edges[MAX_EDGES];
    int edge_count = 0;
    
    for (int i = 0; i < g->num_nodes; i++) {
        for (int j = i + 1; j < g->num_nodes; j++) {
            if (g->adjacency[i][j]) {
                if (edge_count >= MAX_EDGES) {
                    printf("Too many edges for Kruskal\n");
                    return;
                }
                edges[edge_count].u = i;
                edges[edge_count].v = j;
                edges[edge_count].weight = g->edge_weights[i][j];
                edge_count++;
            }
        }
    }
    
    qsort(edges, edge_count, sizeof(struct Edge), compare_edges);
    
    int parent[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        parent[i] = i;
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int mst_edges = 0;
    int total_weight = 0;
    
    for (int i = 0; i < edge_count && mst_edges < g->num_nodes - 1; i