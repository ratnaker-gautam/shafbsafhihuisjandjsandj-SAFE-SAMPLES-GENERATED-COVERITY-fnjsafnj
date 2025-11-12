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
    if (nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = edges;
    memset(g->edges, 0, sizeof(g->edges));
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
        if (*(rank + root_x) < *(rank + root_y)) {
            *(parent + root_x) = root_y;
        } else if (*(rank + root_x) > *(rank + root_y)) {
            *(parent + root_y) = root_x;
        } else {
            *(parent + root_y) = root_x;
            (*(rank + root_x))++;
        }
    }
}

int compare_edges(const void *a, const void *b) {
    const struct Edge *edge_a = (const struct Edge *)a;
    const struct Edge *edge_b = (const struct Edge *)b;
    return edge_a->weight - edge_b->weight;
}

void kruskal_mst(struct Graph *g) {
    if (g == NULL || g->num_nodes <= 0 || g->num_edges < 0) {
        fprintf(stderr, "Invalid graph\n");
        return;
    }

    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    int e = 0;
    int i = 0;

    for (int v = 0; v < g->num_nodes; v++) {
        *(parent + v) = v;
        *(rank + v) = 0;
    }

    qsort(g->edges, g->num_edges, sizeof(struct Edge), compare_edges);

    while (e < g->num_nodes - 1 && i < g->num_edges) {
        struct Edge *next_edge = g->edges + i;
        i++;

        int x = find_parent(parent, next_edge->src);
        int y = find_parent(parent, next_edge->dest);

        if (x != y) {
            *(result + e) = *next_edge;
            e++;
            union_sets(parent, rank, x, y);
        }
    }

    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int j = 0; j < e; j++) {
        struct Edge *edge_ptr = result + j;
        printf("%d -- %d : %d\n", edge_ptr->src, edge_ptr->dest, edge_ptr->weight);
        total_weight += edge_ptr->weight;
    }
    printf("Total weight: %d\n", total_weight);
}

int main() {
    struct Graph g;
    int nodes, edges;

    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }

    printf("Enter number of edges (0-1000): ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }

    init_graph(&g, nodes, edges);

    printf("Enter edges as 'source destination weight' (0-indexed):\n");
    for (int i = 0; i < edges; i++) {
        struct Edge *edge_ptr = g.edges + i;
        if (scanf("%d %d %d", &edge_ptr->src, &edge_ptr->dest, &edge_ptr->weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (edge_ptr->src < 0 || edge_ptr->src >= nodes || 
            edge_ptr->dest < 0 || edge_ptr->dest >= nodes) {
            fprintf(stderr, "Invalid node index\n");
            return 1;
        }
        if (edge_ptr->weight < 0) {
            fprintf(stderr, "Negative weights not allowed\n");
            return 1;
        }
    }

    kruskal_mst(&g);

    return 0;
}