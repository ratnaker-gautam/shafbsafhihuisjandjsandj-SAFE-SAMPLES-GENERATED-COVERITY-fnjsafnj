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
    int num_nodes;
    int num_edges;
    struct Edge edges[MAX_EDGES];
};

void initialize_graph(struct Graph* graph) {
    graph->num_nodes = 0;
    graph->num_edges = 0;
}

int add_edge(struct Graph* graph, int src, int dest, int weight) {
    if (graph->num_edges >= MAX_EDGES) return 0;
    if (src < 0 || src >= MAX_NODES) return 0;
    if (dest < 0 || dest >= MAX_NODES) return 0;
    if (weight < 0 || weight > 1000) return 0;
    
    graph->edges[graph->num_edges].src = src;
    graph->edges[graph->num_edges].dest = dest;
    graph->edges[graph->num_edges].weight = weight;
    graph->num_edges++;
    
    if (src > graph->num_nodes - 1) graph->num_nodes = src + 1;
    if (dest > graph->num_nodes - 1) graph->num_nodes = dest + 1;
    
    return 1;
}

int find(int parent[], int i) {
    if (parent[i] != i) {
        parent[i] = find(parent, parent[i]);
    }
    return parent[i];
}

void union_sets(int parent[], int rank[], int x, int y) {
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

int compare_edges(const void* a, const void* b) {
    struct Edge* edge1 = (struct Edge*)a;
    struct Edge* edge2 = (struct Edge*)b;
    return edge1->weight - edge2->weight;
}

void kruskal_mst(struct Graph* graph) {
    if (graph->num_nodes == 0 || graph->num_edges == 0) {
        printf("Graph is empty\n");
        return;
    }
    
    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    int e = 0;
    int i = 0;
    
    for (int v = 0; v < graph->num_nodes; v++) {
        parent[v] = v;
        rank[v] = 0;
    }
    
    qsort(graph->edges, graph->num_edges, sizeof(graph->edges[0]), compare_edges);
    
    while (e < graph->num_nodes - 1 && i < graph->num_edges) {
        struct Edge next_edge = graph->edges[i++];
        
        int x = find(parent, next_edge.src);
        int y = find(parent, next_edge.dest);
        
        if (x != y) {
            result[e++] = next_edge;
            union_sets(parent, rank, x, y);
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (i = 0; i < e; i++) {
        printf("%d -- %d (weight: %d)\n", result[i].src, result[i].dest, result[i].weight);
        total_weight += result[i].weight;
    }
    printf("Total weight: %d\n", total_weight);
}

int get_valid_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Graph Minimum Spanning Tree Calculator\n");
    printf("=====================================\n");
    
    int num_edges = get_valid_int("Enter number of edges (1-100): ", 1, 100);
    if (num_edges == -1) return 1;
    
    for (int i = 0; i < num_edges; i++) {
        printf("\nEdge %d:\n", i + 1);
        
        int src = get_valid_int("Enter source node (0-99): ", 0, 99);
        if (src == -1) return 1;
        
        int dest = get_valid_int("Enter destination node (0-99): ", 0, 99);
        if (dest == -1) return 1;
        
        int weight = get_valid_int("Enter edge weight (0-1000): ", 0, 1000);
        if (weight == -1) return 1;
        
        if (!add_edge(&graph, src, dest, weight)) {