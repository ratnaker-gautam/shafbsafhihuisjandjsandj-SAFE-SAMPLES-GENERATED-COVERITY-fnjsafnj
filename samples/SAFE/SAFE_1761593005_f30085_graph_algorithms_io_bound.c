//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void initGraph(struct Graph* graph, int V) {
    if (V <= 0 || V > MAX_NODES) {
        fprintf(stderr, "Invalid number of vertices\n");
        exit(1);
    }
    graph->V = V;
    graph->E = 0;
}

void addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (graph->E >= MAX_EDGES) {
        fprintf(stderr, "Too many edges\n");
        exit(1);
    }
    if (src < 0 || src >= graph->V || dest < 0 || dest >= graph->V) {
        fprintf(stderr, "Invalid vertex index\n");
        exit(1);
    }
    graph->edges[graph->E].src = src;
    graph->edges[graph->E].dest = dest;
    graph->edges[graph->E].weight = weight;
    graph->E++;
}

int compareEdges(const void* a, const void* b) {
    struct Edge* edgeA = (struct Edge*)a;
    struct Edge* edgeB = (struct Edge*)b;
    if (edgeA->weight < edgeB->weight) return -1;
    if (edgeA->weight > edgeB->weight) return 1;
    return 0;
}

int find(int parent[], int i) {
    if (parent[i] == -1) return i;
    return find(parent, parent[i]);
}

void unionSets(int parent[], int x, int y) {
    int xset = find(parent, x);
    int yset = find(parent, y);
    if (xset != yset) parent[xset] = yset;
}

void kruskalMST(struct Graph* graph) {
    struct Edge result[MAX_NODES];
    int e = 0;
    int i = 0;
    
    qsort(graph->edges, graph->E, sizeof(struct Edge), compareEdges);
    
    int parent[MAX_NODES];
    for (int v = 0; v < graph->V; v++) parent[v] = -1;
    
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
    int totalWeight = 0;
    for (i = 0; i < e; i++) {
        printf("%d -- %d (weight: %d)\n", result[i].src, result[i].dest, result[i].weight);
        totalWeight += result[i].weight;
    }
    printf("Total weight: %d\n", totalWeight);
}

int main() {
    struct Graph graph;
    int V, E;
    
    printf("Enter number of vertices (1-%d): ", MAX_NODES);
    if (scanf("%d", &V) != 1 || V < 1 || V > MAX_NODES) {
        fprintf(stderr, "Invalid input for vertices\n");
        return 1;
    }
    
    initGraph(&graph, V);
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &E) != 1 || E < 0 || E > MAX_EDGES) {
        fprintf(stderr, "Invalid input for edges\n");
        return 1;
    }
    
    printf("Enter edges as 'source destination weight' (0-indexed vertices):\n");
    for (int i = 0; i < E; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (src < 0 || src >= V || dest < 0 || dest >= V) {
            fprintf(stderr, "Invalid vertex index in edge\n");
            return 1;
        }
        addEdge(&graph, src, dest, weight);
    }
    
    kruskalMST(&graph);
    
    return 0;
}