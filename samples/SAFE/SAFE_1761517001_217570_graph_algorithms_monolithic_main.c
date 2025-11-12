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
    int numNodes;
    int numEdges;
    struct Edge edges[MAX_EDGES];
};

void initializeGraph(struct Graph* graph, int nodes, int edges) {
    if (nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    graph->numNodes = nodes;
    graph->numEdges = edges;
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

int compareEdges(const void* a, const void* b) {
    struct Edge* edge1 = (struct Edge*)a;
    struct Edge* edge2 = (struct Edge*)b;
    if (edge1->weight < edge2->weight) return -1;
    if (edge1->weight > edge2->weight) return 1;
    return 0;
}

void kruskalMST(struct Graph* graph) {
    if (graph->numNodes <= 0 || graph->numEdges < 0) {
        fprintf(stderr, "Invalid graph\n");
        return;
    }
    
    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    int e = 0;
    int i = 0;
    
    for (int v = 0; v < graph->numNodes; v++) {
        parent[v] = v;
        rank[v] = 0;
    }
    
    qsort(graph->edges, graph->numEdges, sizeof(graph->edges[0]), compareEdges);
    
    while (e < graph->numNodes - 1 && i < graph->numEdges) {
        struct Edge next_edge = graph->edges[i++];
        
        int x = find(parent, next_edge.src);
        int y = find(parent, next_edge.dest);
        
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
    int numNodes, numEdges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &numNodes) != 1 || numNodes < 1 || numNodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &numEdges) != 1 || numEdges < 0 || numEdges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    if (numEdges > (numNodes * (numNodes - 1)) / 2) {
        fprintf(stderr, "Too many edges for given nodes\n");
        return 1;
    }
    
    initializeGraph(&graph, numNodes, numEdges);
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < numEdges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (src < 0 || src >= numNodes || dest < 0 || dest >= numNodes) {
            fprintf(stderr, "Invalid node index\n");
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