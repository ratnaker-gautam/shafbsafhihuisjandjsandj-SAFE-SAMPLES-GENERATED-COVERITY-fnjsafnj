//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: graph_algorithms
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

void initializeGraph(struct Graph* graph, int V, int E) {
    if (V <= 0 || V > MAX_NODES || E < 0 || E > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    graph->V = V;
    graph->E = E;
}

int validateEdge(int src, int dest, int weight, int V) {
    if (src < 0 || src >= V || dest < 0 || dest >= V) {
        return 0;
    }
    if (weight < -1000 || weight > 1000) {
        return 0;
    }
    return 1;
}

void addEdge(struct Graph* graph, int index, int src, int dest, int weight) {
    if (index < 0 || index >= graph->E) {
        fprintf(stderr, "Invalid edge index\n");
        exit(1);
    }
    if (!validateEdge(src, dest, weight, graph->V)) {
        fprintf(stderr, "Invalid edge parameters\n");
        exit(1);
    }
    graph->edges[index].src = src;
    graph->edges[index].dest = dest;
    graph->edges[index].weight = weight;
}

int compareEdges(const void* a, const void* b) {
    struct Edge* edgeA = (struct Edge*)a;
    struct Edge* edgeB = (struct Edge*)b;
    if (edgeA->weight < edgeB->weight) return -1;
    if (edgeA->weight > edgeB->weight) return 1;
    return 0;
}

int find(int parent[], int i) {
    if (parent[i] != i) {
        parent[i] = find(parent, parent[i]);
    }
    return parent[i];
}

void unionSets(int parent[], int rank[], int x, int y) {
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

void kruskalMST(struct Graph* graph) {
    int V = graph->V;
    struct Edge result[V];
    int e = 0;
    int i = 0;
    
    qsort(graph->edges, graph->E, sizeof(struct Edge), compareEdges);
    
    int parent[V];
    int rank[V];
    for (int v = 0; v < V; v++) {
        parent[v] = v;
        rank[v] = 0;
    }
    
    while (e < V - 1 && i < graph->E) {
        struct Edge next_edge = graph->edges[i++];
        int x = find(parent, next_edge.src);
        int y = find(parent, next_edge.dest);
        
        if (x != y) {
            result[e++] = next_edge;
            unionSets(parent, rank, x, y);
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
    if (scanf("%d", &V) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &E) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (V <= 0 || V > MAX_NODES || E < 0 || E > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        return 1;
    }
    
    initializeGraph(&graph, V, E);
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < E; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        addEdge(&graph, i, src, dest, weight);
    }
    
    kruskalMST(&graph);
    
    return 0;
}