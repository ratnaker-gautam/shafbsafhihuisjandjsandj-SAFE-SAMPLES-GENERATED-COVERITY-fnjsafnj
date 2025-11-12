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
    int numNodes;
    int numEdges;
    struct Edge edges[MAX_EDGES];
};

void initializeGraph(struct Graph* graph, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        exit(1);
    }
    graph->numNodes = nodes;
    graph->numEdges = 0;
}

int addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (graph->numEdges >= MAX_EDGES) {
        return 0;
    }
    if (src < 0 || src >= graph->numNodes || dest < 0 || dest >= graph->numNodes) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    
    graph->edges[graph->numEdges].src = src;
    graph->edges[graph->numEdges].dest = dest;
    graph->edges[graph->numEdges].weight = weight;
    graph->numEdges++;
    return 1;
}

void printGraph(struct Graph* graph) {
    printf("Graph with %d nodes and %d edges:\n", graph->numNodes, graph->numEdges);
    for (int i = 0; i < graph->numEdges; i++) {
        printf("Edge %d-%d weight: %d\n", 
               graph->edges[i].src, 
               graph->edges[i].dest, 
               graph->edges[i].weight);
    }
}

int compareEdges(const void* a, const void* b) {
    struct Edge* edgeA = (struct Edge*)a;
    struct Edge* edgeB = (struct Edge*)b;
    if (edgeA->weight < edgeB->weight) return -1;
    if (edgeA->weight > edgeB->weight) return 1;
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
    if (graph->numNodes == 0) {
        printf("Graph is empty\n");
        return;
    }
    
    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    int edgeCount = 0;
    int i = 0;
    
    for (int v = 0; v < graph->numNodes; v++) {
        parent[v] = v;
        rank[v] = 0;
    }
    
    struct Edge sortedEdges[MAX_EDGES];
    memcpy(sortedEdges, graph->edges, graph->numEdges * sizeof(struct Edge));
    qsort(sortedEdges, graph->numEdges, sizeof(struct Edge), compareEdges);
    
    printf("Minimum Spanning Tree (Kruskal's algorithm):\n");
    int totalWeight = 0;
    
    while (edgeCount < graph->numNodes - 1 && i < graph->numEdges) {
        struct Edge nextEdge = sortedEdges[i++];
        
        int x = findParent(parent, nextEdge.src);
        int y = findParent(parent, nextEdge.dest);
        
        if (x != y) {
            result[edgeCount++] = nextEdge;
            unionSets(parent, rank, x, y);
            printf("Edge %d-%d weight: %d\n", nextEdge.src, nextEdge.dest, nextEdge.weight);
            totalWeight += nextEdge.weight;
        }
    }
    
    printf("Total MST weight: %d\n", totalWeight);
}

int main() {
    struct Graph graph;
    int numNodes, numEdges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &numNodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    initializeGraph(&graph, numNodes);
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &numEdges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (numEdges < 0 || numEdges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as: source destination weight\n");
    for (int i = 0; i < numEdges; i++) {
        int src, dest, weight;
        printf("Edge %d: ", i + 1);
        if (scanf("%d %d