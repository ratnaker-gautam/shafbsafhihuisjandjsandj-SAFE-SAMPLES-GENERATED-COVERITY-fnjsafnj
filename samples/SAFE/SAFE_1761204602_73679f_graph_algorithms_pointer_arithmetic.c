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

void init_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = 0;
}

void add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g->num_edges >= MAX_EDGES) {
        fprintf(stderr, "Too many edges\n");
        exit(1);
    }
    if (src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    struct Edge *e = g->edges + g->num_edges;
    e->src = src;
    e->dest = dest;
    e->weight = weight;
    g->num_edges++;
}

int compare_edges(const void *a, const void *b) {
    const struct Edge *ea = (const struct Edge *)a;
    const struct Edge *eb = (const struct Edge *)b;
    if (ea->weight < eb->weight) return -1;
    if (ea->weight > eb->weight) return 1;
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
        printf("%d - %d : %d\n", edge_ptr->src, edge_ptr->dest, edge_ptr->weight);
        total_weight += edge_ptr->weight;
    }
    printf("Total weight: %d\n", total_weight);
}

int main() {
    struct Graph g;
    int num_nodes, num_edges;

    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    init_graph(&g, num_nodes);

    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &num_edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    if (num_edges < 0 || num_edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }

    printf("Enter edges as src dest weight (node indices 0 to %d):\n", num_nodes - 1);
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&g, src, dest, weight);
    }

    kruskal_mst(&g);

    return 0;
}