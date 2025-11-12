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

int validateNode(struct Graph* graph, int node) {
    return node >= 0 && node < graph->numNodes;
}

void addEdge(struct Graph* graph, int index, int src, int dest, int weight) {
    if (index < 0 || index >= graph->numEdges) {
        fprintf(stderr, "Invalid edge index\n");
        exit(1);
    }
    if (!validateNode(graph, src) || !validateNode(graph, dest)) {
        fprintf(stderr, "Invalid node in edge\n");
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
    if (graph->numNodes <= 0 || graph->numEdges < 0) {
        fprintf(stderr, "Invalid graph for MST\n");
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
    
    struct Edge sortedEdges[MAX_EDGES];
    for (int j = 0; j < graph->numEdges; j++) {
        sortedEdges[j] = graph->edges[j];
    }
    qsort(sortedEdges, graph->numEdges, sizeof(struct Edge), compareEdges);
    
    while (e < graph->numNodes - 1 && i < graph->numEdges) {
        struct Edge nextEdge = sortedEdges[i++];
        
        int x = findParent(parent, nextEdge.src);
        int y = findParent(parent, nextEdge.dest);
        
        if (x != y) {
            result[e++] = nextEdge;
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

void dijkstra(struct Graph* graph, int startNode) {
    if (!validateNode(graph, startNode)) {
        fprintf(stderr, "Invalid start node for Dijkstra\n");
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->numNodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[startNode] = 0;
    
    for (int count = 0; count < graph->numNodes - 1; count++) {
        int minDist = INT_MAX;
        int minNode = -1;
        
        for (int v = 0; v < graph->numNodes; v++) {
            if (!visited[v] && dist[v] <= minDist) {
                minDist = dist[v];
                minNode = v;
            }
        }
        
        if (minNode == -1) break;
        
        visited[minNode] = 1;
        
        for (int i = 0; i < graph->numEdges; i++) {
            if (graph->edges[i].src == minNode) {
                int v = graph->edges[i].dest;
                int weight = graph->edges[i].weight;
                if