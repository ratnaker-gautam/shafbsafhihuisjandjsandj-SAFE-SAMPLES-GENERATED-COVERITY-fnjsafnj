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
    int V;
    int E;
    struct Edge edges[MAX_EDGES];
};

void initGraph(struct Graph* graph, int V) {
    if (V <= 0 || V > MAX_NODES) {
        V = MAX_NODES;
    }
    graph->V = V;
    graph->E = 0;
}

int addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (graph->E >= MAX_EDGES) {
        return 0;
    }
    if (src < 0 || src >= graph->V || dest < 0 || dest >= graph->V) {
        return 0;
    }
    if (weight < 0 || weight > 1000) {
        return 0;
    }
    graph->edges[graph->E].src = src;
    graph->edges[graph->E].dest = dest;
    graph->edges[graph->E].weight = weight;
    graph->E++;
    return 1;
}

int compareEdges(const void* a, const void* b) {
    struct Edge* edgeA = (struct Edge*)a;
    struct Edge* edgeB = (struct Edge*)b;
    if (edgeA->weight < edgeB->weight) return -1;
    if (edgeA->weight > edgeB->weight) return 1;
    return 0;
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

void kruskalMST(struct Graph* graph) {
    if (graph->V <= 0 || graph->E <= 0) {
        printf("Graph is empty or invalid\n");
        return;
    }
    
    struct Edge result[graph->V];
    int parent[graph->V];
    int rank[graph->V];
    int e = 0;
    int i = 0;
    
    qsort(graph->edges, graph->E, sizeof(graph->edges[0]), compareEdges);
    
    for (int v = 0; v < graph->V; v++) {
        parent[v] = v;
        rank[v] = 0;
    }
    
    while (e < graph->V - 1 && i < graph->E) {
        struct Edge next_edge = graph->edges[i++];
        int x = find(parent, next_edge.src);
        int y = find(parent, next_edge.dest);
        
        if (x != y) {
            result[e++] = next_edge;
            unionSets(parent, rank, x, y);
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int totalWeight = 0;
    for (i = 0; i < e; i++) {
        printf("%d -- %d == %d\n", result[i].src, result[i].dest, result[i].weight);
        totalWeight += result[i].weight;
    }
    printf("Total weight: %d\n", totalWeight);
}

int getValidInt(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number between %d and %d\n", min, max);
    }
}

int main() {
    struct Graph graph;
    int V, E;
    
    printf("Graph Minimum Spanning Tree (Kruskal's Algorithm)\n");
    printf("=================================================\n");
    
    V = getValidInt("Enter number of nodes (1-100): ", 1, MAX_NODES);
    initGraph(&graph, V);
    
    E = getValidInt("Enter number of edges: ", 0, MAX_EDGES);
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < E; i++) {
        printf("Edge %d: ", i + 1);
        
        int src, dest, weight;
        char buffer[100];
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (sscanf(buffer, "%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge format. Please enter: source destination weight\n");
            i--;
            continue;
        }
        
        if (!addEdge(&graph, src, dest, weight)) {
            printf("Invalid edge parameters. Source and destination must be