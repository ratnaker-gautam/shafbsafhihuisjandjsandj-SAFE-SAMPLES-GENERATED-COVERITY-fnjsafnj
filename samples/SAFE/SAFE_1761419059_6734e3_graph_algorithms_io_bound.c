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

void initialize_graph(struct Graph *graph) {
    graph->num_nodes = 0;
    graph->num_edges = 0;
}

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph->num_edges >= MAX_EDGES) return 0;
    if (src < 0 || dest < 0 || src >= MAX_NODES || dest >= MAX_NODES) return 0;
    
    graph->edges[graph->num_edges].src = src;
    graph->edges[graph->num_edges].dest = dest;
    graph->edges[graph->num_edges].weight = weight;
    graph->num_edges++;
    return 1;
}

int compare_edges(const void *a, const void *b) {
    struct Edge *edgeA = (struct Edge *)a;
    struct Edge *edgeB = (struct Edge *)b;
    return edgeA->weight - edgeB->weight;
}

int find_parent(int parent[], int node) {
    if (parent[node] != node) {
        parent[node] = find_parent(parent, parent[node]);
    }
    return parent[node];
}

void kruskal_mst(struct Graph *graph) {
    if (graph->num_nodes == 0 || graph->num_edges == 0) {
        printf("Graph is empty\n");
        return;
    }
    
    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int result_count = 0;
    
    for (int i = 0; i < graph->num_nodes; i++) {
        parent[i] = i;
    }
    
    qsort(graph->edges, graph->num_edges, sizeof(struct Edge), compare_edges);
    
    for (int i = 0; i < graph->num_edges && result_count < graph->num_nodes - 1; i++) {
        int src_parent = find_parent(parent, graph->edges[i].src);
        int dest_parent = find_parent(parent, graph->edges[i].dest);
        
        if (src_parent != dest_parent) {
            result[result_count] = graph->edges[i];
            result_count++;
            parent[src_parent] = dest_parent;
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int i = 0; i < result_count; i++) {
        printf("%d - %d : %d\n", result[i].src, result[i].dest, result[i].weight);
        total_weight += result[i].weight;
    }
    printf("Total weight: %d\n", total_weight);
}

int get_valid_int(const char *prompt, int min, int max) {
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
    
    graph.num_nodes = get_valid_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    if (graph.num_nodes == -1) return 1;
    
    int num_edges = get_valid_int("Enter number of edges: ", 0, MAX_EDGES);
    if (num_edges == -1) return 1;
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < num_edges; i++) {
        printf("Edge %d: ", i + 1);
        
        int src, dest, weight;
        char buffer[100];
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input\n");
            return 1;
        }
        
        if (sscanf(buffer, "%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge format. Please enter: source destination weight\n");
            i--;
            continue;
        }
        
        if (src < 0 || src >= graph.num_nodes || dest < 0 || dest >= graph.num_nodes) {
            printf("Node indices must be between 0 and %d\n", graph.num_nodes - 1);
            i--;
            continue;
        }
        
        if (weight < 0 || weight > 10000) {
            printf("Weight must be between 0 and 10000\n");
            i--;
            continue;
        }
        
        if (!add_edge(&graph, src, dest, weight)) {
            printf("Failed to add edge. Maximum edges reached.\n");
            break;
        }
    }
    
    printf("\n");
    kruskal_mst(&graph);
    
    return 0;
}