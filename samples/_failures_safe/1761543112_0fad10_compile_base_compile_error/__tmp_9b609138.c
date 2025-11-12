//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
    if (graph == NULL || nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        return;
    }
    graph->numNodes = nodes;
    graph->numEdges = edges;
    for (int i = 0; i < edges; i++) {
        graph->edges[i].src = 0;
        graph->edges[i].dest = 0;
        graph->edges[i].weight = 0;
    }
}

int validateEdge(int src, int dest, int weight, int numNodes) {
    if (src < 0 || src >= numNodes || dest < 0 || dest >= numNodes) {
        return 0;
    }
    if (weight < -1000 || weight > 1000) {
        return 0;
    }
    return 1;
}

int addEdge(struct Graph* graph, int edgeIndex, int src, int dest, int weight) {
    if (graph == NULL || edgeIndex < 0 || edgeIndex >= graph->numEdges) {
        return 0;
    }
    if (!validateEdge(src, dest, weight, graph->numNodes)) {
        return 0;
    }
    graph->edges[edgeIndex].src = src;
    graph->edges[edgeIndex].dest = dest;
    graph->edges[edgeIndex].weight = weight;
    return 1;
}

void printGraph(struct Graph* graph) {
    if (graph == NULL) {
        return;
    }
    printf("Graph with %d nodes and %d edges:\n", graph->numNodes, graph->numEdges);
    for (int i = 0; i < graph->numEdges; i++) {
        printf("Edge %d: %d -> %d (weight %d)\n", i, graph->edges[i].src, graph->edges[i].dest, graph->edges[i].weight);
    }
}

int compareEdges(const void* a, const void* b) {
    const struct Edge* edgeA = (const struct Edge*)a;
    const struct Edge* edgeB = (const struct Edge*)b;
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

int unionSets(int parent[], int rank[], int x, int y) {
    int rootX = findParent(parent, x);
    int rootY = findParent(parent, y);
    if (rootX == rootY) {
        return 0;
    }
    if (rank[rootX] < rank[rootY]) {
        parent[rootX] = rootY;
    } else if (rank[rootX] > rank[rootY]) {
        parent[rootY] = rootX;
    } else {
        parent[rootY] = rootX;
        rank[rootX]++;
    }
    return 1;
}

void kruskalMST(struct Graph* graph) {
    if (graph == NULL || graph->numNodes <= 0 || graph->numEdges <= 0) {
        printf("Invalid graph for MST\n");
        return;
    }
    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    for (int i = 0; i < graph->numNodes; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
    qsort(graph->edges, graph->numEdges, sizeof(struct Edge), compareEdges);
    int edgeCount = 0;
    int resultIndex = 0;
    while (edgeCount < graph->numNodes - 1 && resultIndex < graph->numEdges) {
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
        printf("%d -- %d (weight %d)\n", result[i].src, result[i].dest, result[i].weight);
        totalWeight += result[i].weight;
    }
    printf("Total MST weight: %d\n", totalWeight);
}

int main() {
    struct Graph graph;
    int numNodes, numEdges;
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &numNodes) != 1 || numNodes < 1 || numNodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    printf("Enter number of edges (0-1000): ");
    if (scanf("%d", &numEdges