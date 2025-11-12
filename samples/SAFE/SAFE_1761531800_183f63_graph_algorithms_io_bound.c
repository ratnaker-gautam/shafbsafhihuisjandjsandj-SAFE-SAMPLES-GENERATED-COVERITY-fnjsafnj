//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
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

void add_edge(struct Graph *g, int u, int v, int weight) {
    if (g->edge_count >= MAX_EDGES) {
        fprintf(stderr, "Too many edges\n");
        exit(1);
    }
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    g->edges[g->edge_count][0] = u;
    g->edges[g->edge_count][1] = v;
    g->edges[g->edge_count][2] = weight;
    g->edge_count++;
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
    
    int temp_edges[MAX_EDGES][3];
    memcpy(temp_edges, g->edges, sizeof(int) * 3 * g->edge_count);
    
    qsort(temp_edges, g->edge_count, sizeof(int) * 3, compare_edges);
    
    printf("Minimum Spanning Tree edges:\n");
    int mst_edges = 0;
    int total_weight = 0;
    
    for (int i = 0; i < g->edge_count && mst_edges < g->nodes - 1; i++) {
        int u = temp_edges[i][0];
        int v = temp_edges[i][1];
        int weight = temp_edges[i][2];
        
        int set_u = find(parent, u);
        int set_v = find(parent, v);
        
        if (set_u != set_v) {
            printf("Edge %d-%d with weight %d\n", u, v, weight);
            total_weight += weight;
            mst_edges++;
            union_sets(parent, rank, set_u, set_v);
        }
    }
    
    if (mst_edges == g->nodes - 1) {
        printf("Total MST weight: %d\n", total_weight);
    } else {
        printf("Graph is not connected\n");
    }
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-100): ");
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
    
    printf("Enter edges as 'u v weight' (nodes 0-indexed):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&g, u, v, weight);
    }
    
    kruskal_mst(&g);
    
    return 0;
}