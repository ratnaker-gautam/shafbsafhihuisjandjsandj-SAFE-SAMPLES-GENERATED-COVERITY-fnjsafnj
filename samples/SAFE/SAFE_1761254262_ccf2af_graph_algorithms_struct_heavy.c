//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
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
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) return NULL;
    graph->V = V;
    graph->E = E;
    graph->edges = malloc(E * sizeof(struct Edge));
    if (graph->edges == NULL) {
        free(graph);
        return NULL;
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
    struct Subset* subsets = malloc(V * sizeof(struct Subset));
    if (subsets == NULL) return;
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
    printf("Minimum Cost: %d\n", minimumCost);
    free(subsets);
}

int main() {
    int V = 4;
    int E = 5;
    struct Graph* graph = createGraph(V, E);
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    graph->edges[0].src = 0;
    graph->edges[0].dest = 1;
    graph->edges[0].weight = 10;
    graph->edges[1].src = 0;
    graph->edges[1].dest = 2;
    graph->edges[1].weight = 6;
    graph->edges[2].src = 0;
    graph->edges[2].dest = 3;
    graph->edges[2].weight = 5;
    graph->edges[3].src = 1;
    graph->edges[3].dest = 3;
    graph->edges[3].weight = 15;
    graph->edges[4].src = 2;
    graph->edges[4].dest = 3;
    graph->edges[4].weight = 4;
    KruskalMST(graph);
    free(graph->edges);
    free(graph);
    return 0;
}