//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Edge {
    int src;
    int dest;
    int weight;
};

struct Graph {
    int V;
    int E;
    struct Edge edges[MAX_EDGES];
};

void initGraph(struct Graph* graph, int V, int E) {
    if (V <= 0 || V > MAX_NODES || E < 0 || E > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    graph->V = V;
    graph->E = E;
}

int find(int parent[], int i) {
    while (parent[i] != -1) {
        i = parent[i];
    }
    return i;
}

void unionSets(int parent[], int x, int y) {
    int xset = find(parent, x);
    int yset = find(parent, y);
    if (xset != yset) {
        parent[xset] = yset;
    }
}

int compareEdges(const void* a, const void* b) {
    struct Edge* edge1 = (struct Edge*)a;
    struct Edge* edge2 = (struct Edge*)b;
    if (edge1->weight < edge2->weight) return -1;
    if (edge1->weight > edge2->weight) return 1;
    return 0;
}

void kruskalMST(struct Graph* graph) {
    if (graph->V <= 0 || graph->E < 0) {
        fprintf(stderr, "Invalid graph\n");
        return;
    }

    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int e = 0;
    int i = 0;

    for (int v = 0; v < graph->V; v++) {
        parent[v] = -1;
    }

    qsort(graph->edges, graph->E, sizeof(graph->edges[0]), compareEdges);

    while (e < graph->V - 1 && i < graph->E) {
        struct Edge next_edge = graph->edges[i++];

        int x = find(parent, next_edge.src);
        int y = find(parent, next_edge.dest);

        if (x != y) {
            result[e++] = next_edge;
            unionSets(parent, x, y);
        }
    }

    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int j = 0; j < e; j++) {
        printf("%d -- %d : %d\n", result[j].src, result[j].dest, result[j].weight);
        total_weight += result[j].weight;
    }
    printf("Total weight: %d\n", total_weight);
}

int main() {
    struct Graph graph;
    int V, E;

    printf("Enter number of vertices (1-%d): ", MAX_NODES);
    if (scanf("%d", &V) != 1 || V < 1 || V > MAX_NODES) {
        fprintf(stderr, "Invalid number of vertices\n");
        return 1;
    }

    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &E) != 1 || E < 0 || E > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }

    if (E > V * (V - 1) / 2) {
        fprintf(stderr, "Too many edges for given vertices\n");
        return 1;
    }

    initGraph(&graph, V, E);

    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < E; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (src < 0 || src >= V || dest < 0 || dest >= V) {
            fprintf(stderr, "Invalid vertex index\n");
            return 1;
        }
        if (weight < 0) {
            fprintf(stderr, "Negative weights not allowed\n");
            return 1;
        }
        graph.edges[i].src = src;
        graph.edges[i].dest = dest;
        graph.edges[i].weight = weight;
    }

    kruskalMST(&graph);

    return 0;
}