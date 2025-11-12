//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    graph->numNodes = nodes;
    graph->numEdges = 0;
}

void addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (graph->numEdges >= MAX_EDGES) {
        fprintf(stderr, "Too many edges\n");
        exit(1);
    }
    if (src < 0 || src >= graph->numNodes || dest < 0 || dest >= graph->numNodes) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    graph->edges[graph->numEdges].src = src;
    graph->edges[graph->numEdges].dest = dest;
    graph->edges[graph->numEdges].weight = weight;
    graph->numEdges++;
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
    if (graph->numNodes <= 0 || graph->numEdges <= 0) {
        printf("Graph is empty\n");
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
    
    qsort(graph->edges, graph->numEdges, sizeof(struct Edge), compareEdges);
    
    while (e < graph->numNodes - 1 && i < graph->numEdges) {
        struct Edge nextEdge = graph->edges[i++];
        
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
        printf("%d -- %d (weight: %d)\n", result[j].src, result[j].dest, result[j].weight);
        totalWeight += result[j].weight;
    }
    printf("Total weight: %d\n", totalWeight);
}

int main() {
    struct Graph graph;
    int numNodes, numEdges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &numNodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initializeGraph(&graph, numNodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &numEdges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (numEdges < 0 || numEdges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as: source destination weight\n");
    for (int i = 0; i < numEdges; i++) {
        int src, dest, weight;
        printf("Edge %d: ", i + 1);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        addEdge(&graph, src, dest, weight);
    }
    
    kruskalMST(&graph);
    
    return 0;
}