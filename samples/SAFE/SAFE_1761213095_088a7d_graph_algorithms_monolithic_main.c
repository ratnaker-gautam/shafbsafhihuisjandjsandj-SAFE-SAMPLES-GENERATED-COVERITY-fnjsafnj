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
    return edgeA->weight - edgeB->weight;
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
    int e = 0;
    int i = 0;
    
    for (int v = 0; v < graph->V; v++) {
        parent[v] = v;
        rank[v] = 0;
    }
    
    qsort(graph->edges, graph->E, sizeof(struct Edge), compareEdges);
    
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
    for (int j = 0; j < e; j++) {
        printf("%d -- %d : %d\n", result[j].src, result[j].dest, result[j].weight);
        totalWeight += result[j].weight;
    }
    printf("Total weight: %d\n", totalWeight);
}

int main() {
    struct Graph graph;
    int V, E;
    
    printf("Enter number of vertices (1-%d): ", MAX_NODES);
    if (scanf("%d", &V) != 1 || V <= 0 || V > MAX_NODES) {
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
    
    printf("Enter edges as 'source destination weight' (0-indexed vertices):\n");
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