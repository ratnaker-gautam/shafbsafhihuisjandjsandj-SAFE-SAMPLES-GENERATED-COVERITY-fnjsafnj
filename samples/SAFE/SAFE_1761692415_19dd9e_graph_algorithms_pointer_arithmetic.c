//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 500

struct Graph {
    int num_nodes;
    int num_edges;
    int adj_matrix[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = 0;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            *(*(g->adj_matrix + i) + j) = 0;
        }
    }
}

int add_edge(struct Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->num_nodes || v < 0 || v >= g->num_nodes) {
        return 0;
    }
    if (u == v) {
        return 0;
    }
    if (*(*(g->adj_matrix + u) + v) != 0) {
        return 0;
    }
    if (g->num_edges >= MAX_EDGES) {
        return 0;
    }
    *(*(g->adj_matrix + u) + v) = weight;
    *(*(g->adj_matrix + v) + u) = weight;
    g->num_edges++;
    return 1;
}

void depth_first_search(struct Graph *g, int start, int *visited) {
    if (start < 0 || start >= g->num_nodes) {
        return;
    }
    int stack[MAX_NODES];
    int top = 0;
    *(stack + top) = start;
    *(visited + start) = 1;
    
    while (top >= 0) {
        int current = *(stack + top);
        top--;
        printf("%d ", current);
        
        for (int i = 0; i < g->num_nodes; i++) {
            int weight = *(*(g->adj_matrix + current) + i);
            if (weight != 0 && *(visited + i) == 0) {
                top++;
                *(stack + top) = i;
                *(visited + i) = 1;
            }
        }
    }
}

int main() {
    struct Graph graph;
    int nodes, edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initialize_graph(&graph, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as u v weight (0-indexed):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&graph, u, v, w)) {
            fprintf(stderr, "Failed to add edge %d %d\n", u, v);
            return 1;
        }
    }
    
    int visited[MAX_NODES];
    for (int i = 0; i < graph.num_nodes; i++) {
        *(visited + i) = 0;
    }
    
    printf("DFS traversal starting from node 0: ");
    depth_first_search(&graph, 0, visited);
    printf("\n");
    
    int unreachable = 0;
    for (int i = 0; i < graph.num_nodes; i++) {
        if (*(visited + i) == 0) {
            unreachable++;
        }
    }
    
    if (unreachable > 0) {
        printf("Warning: %d nodes were unreachable from node 0\n", unreachable);
    }
    
    return 0;
}