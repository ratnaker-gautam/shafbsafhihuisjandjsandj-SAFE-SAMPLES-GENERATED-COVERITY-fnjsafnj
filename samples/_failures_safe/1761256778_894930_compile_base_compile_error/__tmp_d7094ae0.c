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

int validateEdge(int src, int dest, int weight, int V) {
    if (src < 0 || src >= V || dest < 0 || dest >= V) {
        return 0;
    }
    if (weight < 0 || weight > 1000) {
        return 0;
    }
    return 1;
}

int addEdge(struct Graph* graph, int idx, int src, int dest, int weight) {
    if (idx < 0 || idx >= graph->E) {
        return 0;
    }
    if (!validateEdge(src, dest, weight, graph->V)) {
        return 0;
    }
    graph->edges[idx].src = src;
    graph->edges[idx].dest = dest;
    graph->edges[idx].weight = weight;
    return 1;
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
    if (graph->V <= 0 || graph->E <= 0) {
        printf("Invalid graph parameters\n");
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
    int V, E;

    printf("Enter number of vertices (1-%d): ", MAX_NODES);
    if (scanf("%d", &V) != 1 || V < 1 || V > MAX_NODES) {
        printf("Invalid number of vertices\n");
        return 1;
    }

    printf("Enter number of edges (1-%d): ", MAX_EDGES);
    if (scanf("%d", &E) != 1 || E < 1 || E > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }

    if (E > V * (V - 1) / 2) {
        printf("Too many edges for given vertices\n");
        return 1;
    }

    initializeGraph(&graph, V, E);

    printf("Enter edges as: source destination weight\n");
    for (int i = 0; i < E; i++) {
        int src, dest, weight;
        printf("Edge %d: ", i + 1);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!addEdge(&graph, i, src, dest, weight)) {
            printf("Invalid edge parameters\n");
            return 1;
        }
    }

    kruskalMST(&