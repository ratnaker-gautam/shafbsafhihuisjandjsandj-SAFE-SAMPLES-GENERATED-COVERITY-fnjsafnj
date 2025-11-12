//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Edge {
    int src;
    int dest;
    int weight;
};

struct Graph {
    int V;
    int E;
    struct Edge* edges;
};

struct Subset {
    int parent;
    int rank;
};

struct Graph* createGraph(int V, int E) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    if (graph == NULL) {
        exit(EXIT_FAILURE);
    }
    graph->V = V;
    graph->E = E;
    graph->edges = (struct Edge*)malloc(E * sizeof(struct Edge));
    if (graph->edges == NULL) {
        free(graph);
        exit(EXIT_FAILURE);
    }
    return graph;
}

int find(struct Subset subsets[], int i) {
    if (subsets[i].parent != i) {
        subsets[i].parent = find(subsets, subsets[i].parent);
    }
    return subsets[i].parent;
}

void Union(struct Subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);
    if (subsets[xroot].rank < subsets[yroot].rank) {
        subsets[xroot].parent = yroot;
    } else if (subsets[xroot].rank > subsets[yroot].rank) {
        subsets[yroot].parent = xroot;
    } else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

int compareEdges(const void* a, const void* b) {
    struct Edge* a1 = (struct Edge*)a;
    struct Edge* b1 = (struct Edge*)b;
    if (a1->weight > b1->weight) return 1;
    if (a1->weight < b1->weight) return -1;
    return 0;
}

void KruskalMST(struct Graph* graph) {
    int V = graph->V;
    struct Edge result[V];
    int e = 0;
    int i = 0;
    qsort(graph->edges, graph->E, sizeof(graph->edges[0]), compareEdges);
    struct Subset* subsets = (struct Subset*)malloc(V * sizeof(struct Subset));
    if (subsets == NULL) {
        return;
    }
    for (int v = 0; v < V; ++v) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }
    while (e < V - 1 && i < graph->E) {
        struct Edge next_edge = graph->edges[i++];
        int x = find(subsets, next_edge.src);
        int y = find(subsets, next_edge.dest);
        if (x != y) {
            result[e++] = next_edge;
            Union(subsets, x, y);
        }
    }
    printf("Edges in MST:\n");
    int minimumCost = 0;
    for (i = 0; i < e; ++i) {
        printf("%d -- %d == %d\n", result[i].src, result[i].dest, result[i].weight);
        minimumCost += result[i].weight;
    }
    printf("Minimum Cost Spanning Tree: %d\n", minimumCost);
    free(subsets);
}

int main() {
    int V, E;
    printf("Enter number of vertices: ");
    if (scanf("%d", &V) != 1 || V <= 0) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    printf("Enter number of edges: ");
    if (scanf("%d", &E) != 1 || E <= 0) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    if (E > V * (V - 1) / 2) {
        printf("Too many edges\n");
        return EXIT_FAILURE;
    }
    struct Graph* graph = createGraph(V, E);
    printf("Enter edges (src dest weight):\n");
    for (int i = 0; i < E; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            free(graph->edges);
            free(graph);
            return EXIT_FAILURE;
        }
        if (src < 0 || src >= V || dest < 0 || dest >= V) {
            printf("Invalid vertex\n");
            free(graph->edges);
            free(graph);
            return EXIT_FAILURE;
        }
        if (weight < 0) {
            printf("Negative weight not allowed\n");
            free(graph->edges);
            free(graph);
            return EXIT_FAILURE;
        }
        graph->edges[i].src = src;
        graph->edges[i].dest = dest;
        graph->edges[i].weight = weight;
    }
    KruskalMST(graph);
    free(graph->edges);
    free(graph);
    return EXIT_SUCCESS;
}