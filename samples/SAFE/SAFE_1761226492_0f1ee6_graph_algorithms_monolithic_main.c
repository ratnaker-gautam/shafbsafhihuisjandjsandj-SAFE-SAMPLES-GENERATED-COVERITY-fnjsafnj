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
    graph->V = V;
    graph->E = E;
    for (int i = 0; i < E; i++) {
        graph->edges[i].src = 0;
        graph->edges[i].dest = 0;
        graph->edges[i].weight = 0;
    }
}

int validateInput(int V, int E) {
    if (V <= 0 || V > MAX_NODES) return 0;
    if (E < 0 || E > MAX_EDGES) return 0;
    if (E > V * (V - 1) / 2) return 0;
    return 1;
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
        
        int x = findParent(parent, next_edge.src);
        int y = findParent(parent, next_edge.dest);
        
        if (x != y) {
            result[e++] = next_edge;
            unionSets(parent, rank, x, y);
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int minimumCost = 0;
    for (i = 0; i < e; i++) {
        printf("%d -- %d == %d\n", result[i].src, result[i].dest, result[i].weight);
        minimumCost += result[i].weight;
    }
    printf("Total cost: %d\n", minimumCost);
}

int main() {
    struct Graph graph;
    int V, E;
    
    printf("Enter number of vertices (1-%d): ", MAX_NODES);
    if (scanf("%d", &V) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &E) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validateInput(V, E)) {
        printf("Invalid graph parameters\n");
        return 1;
    }
    
    initializeGraph(&graph, V, E);
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < E; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (src < 0 || src >= V || dest < 0 || dest >= V) {
            printf("Invalid node index\n");
            return 1;
        }
        if (weight < 0) {
            printf("Negative weights not allowed\n");
            return 1;
        }
        graph.edges[i].src = src;
        graph.edges[i].dest = dest;
        graph.edges[i].weight = weight;
    }
    
    kruskalMST(&graph);
    
    return 0;
}