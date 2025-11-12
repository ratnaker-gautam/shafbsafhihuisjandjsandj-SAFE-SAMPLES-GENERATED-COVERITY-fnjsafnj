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

void initializeGraph(struct Graph* graph, int vertices, int edges) {
    if (vertices <= 0 || vertices > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    graph->V = vertices;
    graph->E = edges;
    for (int i = 0; i < edges; i++) {
        graph->edges[i].src = 0;
        graph->edges[i].dest = 0;
        graph->edges[i].weight = 0;
    }
}

int compareEdges(const void* a, const void* b) {
    struct Edge* edge1 = (struct Edge*)a;
    struct Edge* edge2 = (struct Edge*)b;
    if (edge1->weight < edge2->weight) return -1;
    if (edge1->weight > edge2->weight) return 1;
    return 0;
}

int findParent(int parent[], int node) {
    if (parent[node] != node) {
        parent[node] = findParent(parent, parent[node]);
    }
    return parent[node];
}

void unionSets(int parent[], int rank[], int x, int y) {
    int rootX = findParent(parent, x);
    int rootY = findParent(parent, y);
    
    if (rootX != rootY) {
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }
}

void kruskalMST(struct Graph* graph) {
    if (graph == NULL || graph->V <= 0 || graph->E < 0) {
        fprintf(stderr, "Invalid graph\n");
        return;
    }
    
    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    
    for (int i = 0; i < graph->V; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
    
    qsort(graph->edges, graph->E, sizeof(struct Edge), compareEdges);
    
    int edgeCount = 0;
    int resultIndex = 0;
    
    while (edgeCount < graph->V - 1 && resultIndex < graph->E) {
        struct Edge nextEdge = graph->edges[resultIndex++];
        
        int x = findParent(parent, nextEdge.src);
        int y = findParent(parent, nextEdge.dest);
        
        if (x != y) {
            result[edgeCount++] = nextEdge;
            unionSets(parent, rank, x, y);
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int totalWeight = 0;
    for (int i = 0; i < edgeCount; i++) {
        printf("%d -- %d : %d\n", result[i].src, result[i].dest, result[i].weight);
        totalWeight += result[i].weight;
    }
    printf("Total weight: %d\n", totalWeight);
}

int main() {
    struct Graph graph;
    int vertices, edges;
    
    printf("Enter number of vertices (1-%d): ", MAX_NODES);
    if (scanf("%d", &vertices) != 1 || vertices < 1 || vertices > MAX_NODES) {
        fprintf(stderr, "Invalid number of vertices\n");
        return 1;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    if (edges > vertices * (vertices - 1) / 2) {
        fprintf(stderr, "Too many edges for given vertices\n");
        return 1;
    }
    
    initializeGraph(&graph, vertices, edges);
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (src < 0 || src >= vertices || dest < 0 || dest >= vertices) {
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