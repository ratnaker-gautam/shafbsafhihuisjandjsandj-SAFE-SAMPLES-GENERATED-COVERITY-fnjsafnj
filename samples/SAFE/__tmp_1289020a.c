//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int nodes;
    int edges[MAX_EDGES][3];
    int edge_count;
};

void init_graph(struct Graph *g, int nodes) {
    if (nodes < 1 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->nodes = nodes;
    g->edge_count = 0;
}

int add_edge(struct Graph *g, int u, int v, int weight) {
    if (g->edge_count >= MAX_EDGES) {
        return 0;
    }
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    g->edges[g->edge_count][0] = u;
    g->edges[g->edge_count][1] = v;
    g->edges[g->edge_count][2] = weight;
    g->edge_count++;
    return 1;
}

void print_graph(struct Graph *g) {
    printf("Graph with %d nodes and %d edges:\n", g->nodes, g->edge_count);
    for (int i = 0; i < g->edge_count; i++) {
        printf("  %d -> %d (weight: %d)\n", 
               g->edges[i][0], g->edges[i][1], g->edges[i][2]);
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
    const int *edge1 = (const int *)a;
    const int *edge2 = (const int *)b;
    if (edge1[2] < edge2[2]) return -1;
    if (edge1[2] > edge2[2]) return 1;
    return 0;
}

void kruskal_mst(struct Graph *g) {
    if (g->nodes == 0) {
        printf("Graph is empty\n");
        return;
    }
    
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    
    for (int i = 0; i < g->nodes; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
    
    struct Graph mst;
    init_graph(&mst, g->nodes);
    
    int edges[MAX_EDGES][3];
    memcpy(edges, g->edges, sizeof(int) * 3 * g->edge_count);
    
    qsort(edges, g->edge_count, sizeof(edges[0]), compare_edges);
    
    int mst_edges = 0;
    for (int i = 0; i < g->edge_count && mst_edges < g->nodes - 1; i++) {
        int u = edges[i][0];
        int v = edges[i][1];
        int weight = edges[i][2];
        
        int set_u = find(parent, u);
        int set_v = find(parent, v);
        
        if (set_u != set_v) {
            if (add_edge(&mst, u, v, weight)) {
                union_sets(parent, rank, set_u, set_v);
                mst_edges++;
            }
        }
    }
    
    printf("Minimum Spanning Tree (Kruskal's algorithm):\n");
    print_graph(&mst);
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    init_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'u v weight' (0-indexed nodes):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&g, u, v, weight)) {
            fprintf(stderr, "Failed to add edge\n");
            return 1;
        }
    }