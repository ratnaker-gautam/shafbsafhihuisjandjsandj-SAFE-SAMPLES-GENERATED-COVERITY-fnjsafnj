//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
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
}

void add_edge(struct Graph *g, int idx, int src, int dest, int weight) {
    if (g == NULL || idx < 0 || idx >= g->num_edges || src < 0 || src >= g->num_nodes || 
        dest < 0 || dest >= g->num_nodes || weight < 0) {
        fprintf(stderr, "Invalid edge parameters\n");
        exit(1);
    }
    (g->edges + idx)->src = src;
    (g->edges + idx)->dest = dest;
    (g->edges + idx)->weight = weight;
}

int find(int parent[], int i) {
    if (parent[i] == -1) return i;
    return find(parent, parent[i]);
}

void union_sets(int parent[], int x, int y) {
    int xset = find(parent, x);
    int yset = find(parent, y);
    if (xset != yset) {
        *(parent + xset) = yset;
    }
}

int compare_edges(const void *a, const void *b) {
    const struct Edge *edge1 = (const struct Edge *)a;
    const struct Edge *edge2 = (const struct Edge *)b;
    if (edge1->weight < edge2->weight) return -1;
    if (edge1->weight > edge2->weight) return 1;
    return 0;
}

void kruskal_mst(struct Graph *g) {
    if (g == NULL || g->num_nodes <= 0 || g->num_edges < 0) {
        fprintf(stderr, "Invalid graph for MST\n");
        return;
    }

    struct Edge result[MAX_NODES];
    int e = 0;
    int i = 0;

    qsort(g->edges, g->num_edges, sizeof(struct Edge), compare_edges);

    int parent[MAX_NODES];
    for (int v = 0; v < g->num_nodes; v++) {
        *(parent + v) = -1;
    }

    while (e < g->num_nodes - 1 && i < g->num_edges) {
        struct Edge next_edge = *(g->edges + i);
        i++;

        int x = find(parent, next_edge.src);
        int y = find(parent, next_edge.dest);

        if (x != y) {
            *(result + e) = next_edge;
            e++;
            union_sets(parent, x, y);
        }
    }

    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int j = 0; j < e; j++) {
        printf("%d -- %d : %d\n", (result + j)->src, (result + j)->dest, (result + j)->weight);
        total_weight += (result + j)->weight;
    }
    printf("Total weight: %d\n", total_weight);
}

int main() {
    struct Graph graph;
    int num_nodes, num_edges;

    printf("Enter number of nodes (max %d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1 || num_nodes <= 0 || num_nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }

    printf("Enter number of edges (max %d): ", MAX_EDGES);
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }

    init_graph(&graph, num_nodes, num_edges);

    printf("Enter edges (src dest weight):\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (src < 0 || src >= num_nodes || dest < 0 || dest >= num_nodes || weight < 0) {
            fprintf(stderr, "Invalid edge parameters\n");
            return 1;
        }
        add_edge(&graph, i, src, dest, weight);
    }

    kruskal_mst(&graph);

    return 0;
}