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

void init_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
}

int add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g->num_edges >= MAX_EDGES) return 0;
    if (src < 0 || dest < 0) return 0;
    if (src >= MAX_NODES || dest >= MAX_NODES) return 0;
    
    struct Edge *e = g->edges + g->num_edges;
    e->src = src;
    e->dest = dest;
    e->weight = weight;
    g->num_edges++;
    
    if (src >= g->num_nodes) g->num_nodes = src + 1;
    if (dest >= g->num_nodes) g->num_nodes = dest + 1;
    
    return 1;
}

int find(int parent[], int i) {
    if (*(parent + i) == -1) return i;
    return find(parent, *(parent + i));
}

void union_sets(int parent[], int x, int y) {
    int xset = find(parent, x);
    int yset = find(parent, y);
    if (xset != yset) *(parent + xset) = yset;
}

int compare_edges(const void *a, const void *b) {
    const struct Edge *e1 = (const struct Edge*)a;
    const struct Edge *e2 = (const struct Edge*)b;
    return e1->weight - e2->weight;
}

void kruskal_mst(struct Graph *g) {
    if (g->num_nodes == 0 || g->num_edges == 0) {
        printf("Graph is empty\n");
        return;
    }
    
    struct Edge result[MAX_NODES];
    int e = 0;
    int i = 0;
    
    qsort(g->edges, g->num_edges, sizeof(struct Edge), compare_edges);
    
    int parent[MAX_NODES];
    for (int *p = parent; p < parent + MAX_NODES; p++) *p = -1;
    
    while (e < g->num_nodes - 1 && i < g->num_edges) {
        struct Edge *next_edge = g->edges + i;
        i++;
        
        int x = find(parent, next_edge->src);
        int y = find(parent, next_edge->dest);
        
        if (x != y) {
            *(result + e) = *next_edge;
            e++;
            union_sets(parent, x, y);
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int j = 0; j < e; j++) {
        struct Edge *edge = result + j;
        printf("%d -- %d : %d\n", edge->src, edge->dest, edge->weight);
        total_weight += edge->weight;
    }
    printf("Total weight: %d\n", total_weight);
}

int main() {
    struct Graph g;
    init_graph(&g);
    
    printf("Enter number of edges: ");
    int num_edges;
    if (scanf("%d", &num_edges) != 1 || num_edges <= 0 || num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'source destination weight' (0-indexed):\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid input\n");
            return 1;
        }
        if (!add_edge(&g, src, dest, weight)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    kruskal_mst(&g);
    
    return 0;
}