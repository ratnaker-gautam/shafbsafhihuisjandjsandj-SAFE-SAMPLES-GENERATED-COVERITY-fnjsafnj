//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
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

void init_graph(struct Graph *g, int nodes, int edges) {
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = edges;
    memset(g->edges, 0, sizeof(g->edges));
}

int compare_edges(const void *a, const void *b) {
    const struct Edge *edge_a = (const struct Edge *)a;
    const struct Edge *edge_b = (const struct Edge *)b;
    return edge_a->weight - edge_b->weight;
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

void kruskal_mst(struct Graph *g) {
    if (g == NULL || g->num_nodes == 0 || g->num_edges == 0) {
        fprintf(stderr, "Invalid graph for MST\n");
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
    
    qsort(g->edges, g->num_edges, sizeof(struct Edge), compare_edges);
    
    while (e < g->num_nodes - 1 && i < g->num_edges) {
        struct Edge next_edge = *(g->edges + i);
        i++;
        
        int x = find_parent(parent, next_edge.src);
        int y = find_parent(parent, next_edge.dest);
        
        if (x != y) {
            *(result + e) = next_edge;
            e++;
            union_sets(parent, rank, x, y);
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int j = 0; j < e; j++) {
        printf("%d - %d : %d\n", (*(result + j)).src, (*(result + j)).dest, (*(result + j)).weight);
        total_weight += (*(result + j)).weight;
    }
    printf("Total weight: %d\n", total_weight);
}

int main() {
    struct Graph g;
    int num_nodes, num_edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1 || num_nodes < 1 || num_nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    if (num_edges > 0) {
        printf("Enter edges as 'source destination weight' (nodes 0-%d):\n", num_nodes - 1);
    }
    
    init_graph(&g, num_nodes, num_edges);
    
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (src < 0 || src >= num_nodes || dest < 0 || dest >= num_nodes) {
            fprintf(stderr, "Invalid node index\n");
            return 1;
        }
        if (weight < 0) {
            fprintf(stderr, "Negative weights not allowed\n");
            return 1;
        }
        (*(g.edges + i)).src = src;
        (*(g.edges + i)).dest = dest;
        (*(g.edges + i)).weight = weight;
    }
    
    kruskal_mst(&g);
    
    return 0;
}