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

int compareEdges(const void* a, const void* b) {
    struct Edge* edgeA = (struct Edge*)a;
    struct Edge* edgeB = (struct Edge*)b;
    if (edgeA->weight < edgeB->weight) return -1;
    if (edgeA->weight > edgeB->weight) return 1;
    return 0;
}

int findParent(int parent[], int i) {
    if (parent[i] != i) {
        parent[i] = findParent(parent, parent[i]);
    }
    return parent[i];
}

void unionSets(int parent[], int rank[], int x, int y) {
    int xroot = findParent(parent, x);
    int yroot = findParent(parent, y);
    
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
    if (graph->V <= 0 || graph->E < 0) {
        fprintf(stderr, "Invalid graph\n");
        return;
    }
    
    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    int e = 0;
    int i = 0;
    
    qsort(graph->edges, graph->E, sizeof(struct Edge), compareEdges);
    
    for (int v = 0; v < graph->V; v++) {
        parent[v] = v;
        rank[v] = 0;
    }
    
    while (e < graph->V - 1 && i < graph->E) {
        struct Edge next_edge = graph->edges[i++];
        
        int x = findParent(parent, next_edge.src);
        int y = findParent(parent, next_edge.dest);
        
        if (x != y) {
            result[e++] = next_edge;
            unionSets(parent, rank, x, y);
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int totalWeight = 0;
    for (i = 0; i < e; i++) {
        printf("%d -- %d : %d\n", result[i].src, result[i].dest, result[i].weight);
        totalWeight += result[i].weight;
    }
    printf("Total weight: %d\n", totalWeight);
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
    
    initializeGraph(&graph, V, E);
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < E; i++) {
        printf("Edge %d: ", i + 1);
        if (scanf("%d %d %d", &graph.edges[i].src, &graph.edges[i].dest, &graph.edges[i].weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (graph.edges[i].src < 0 || graph.edges[i].src >= V || 
            graph.edges[i].dest < 0 || graph.edges[i].dest >= V) {
            fprintf(stderr, "Invalid vertex index\n");
            return 1;
        }
        if (graph.edges[i].weight < 0) {
            fprintf(stderr, "Negative weights not allowed\n");
            return 1;
        }
    }
    
    kruskalMST(&graph);
    
    return 0;
}