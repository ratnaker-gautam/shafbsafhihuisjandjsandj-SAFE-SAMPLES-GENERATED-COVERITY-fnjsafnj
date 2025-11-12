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
    int num_nodes;
    int num_edges;
    struct Edge edges[MAX_EDGES];
};

void initialize_graph(struct Graph *graph) {
    graph->num_nodes = 0;
    graph->num_edges = 0;
}

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph->num_edges >= MAX_EDGES) {
        return 0;
    }
    if (src < 0 || dest < 0 || src >= MAX_NODES || dest >= MAX_NODES) {
        return 0;
    }
    graph->edges[graph->num_edges].src = src;
    graph->edges[graph->num_edges].dest = dest;
    graph->edges[graph->num_edges].weight = weight;
    graph->num_edges++;
    if (src >= graph->num_nodes) {
        graph->num_nodes = src + 1;
    }
    if (dest >= graph->num_nodes) {
        graph->num_nodes = dest + 1;
    }
    return 1;
}

int compare_edges(const void *a, const void *b) {
    struct Edge *edge1 = (struct Edge *)a;
    struct Edge *edge2 = (struct Edge *)b;
    return edge1->weight - edge2->weight;
}

int find_parent(int parent[], int node) {
    if (parent[node] != node) {
        parent[node] = find_parent(parent, parent[node]);
    }
    return parent[node];
}

void union_sets(int parent[], int rank[], int x, int y) {
    int root_x = find_parent(parent, x);
    int root_y = find_parent(parent, y);
    if (root_x != root_y) {
        if (rank[root_x] < rank[root_y]) {
            parent[root_x] = root_y;
        } else if (rank[root_x] > rank[root_y]) {
            parent[root_y] = root_x;
        } else {
            parent[root_y] = root_x;
            rank[root_x]++;
        }
    }
}

void kruskal_mst(struct Graph *graph) {
    if (graph->num_nodes == 0 || graph->num_edges == 0) {
        printf("Graph is empty\n");
        return;
    }
    
    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    int edge_count = 0;
    
    for (int v = 0; v < graph->num_nodes; v++) {
        parent[v] = v;
        rank[v] = 0;
    }
    
    qsort(graph->edges, graph->num_edges, sizeof(struct Edge), compare_edges);
    
    for (int e = 0; e < graph->num_edges && edge_count < graph->num_nodes - 1; e++) {
        struct Edge next_edge = graph->edges[e];
        int x = find_parent(parent, next_edge.src);
        int y = find_parent(parent, next_edge.dest);
        
        if (x != y) {
            result[edge_count] = next_edge;
            edge_count++;
            union_sets(parent, rank, x, y);
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int j = 0; j < edge_count; j++) {
        printf("%d -- %d : %d\n", result[j].src, result[j].dest, result[j].weight);
        total_weight += result[j].weight;
    }
    printf("Total weight: %d\n", total_weight);
}

int read_int(const char *prompt, int min, int max) {
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
        printf("Invalid input. Please enter an integer between %d and %d.\n", min, max);
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Graph Minimum Spanning Tree (Kruskal's Algorithm)\n");
    printf("=================================================\n");
    
    int num_edges = read_int("Enter number of edges: ", 1, MAX_EDGES);
    if (num_edges == -1) {
        return 1;
    }
    
    for (int i = 0; i < num_edges; i++) {
        printf("\nEdge %d:\n", i + 1);
        int src = read_int("Enter source node: ", 0, MAX_NODES - 1);
        int dest = read_int("Enter destination node: ", 0, MAX_NODES - 1);
        int weight = read_int("Enter weight: ", -1000, 1000);
        
        if (src == -1 || dest == -1 || weight == -1) {
            return 1;
        }
        
        if (!add_edge(&graph, src, dest, weight)) {
            printf("Failed to add edge\n");
            return 1