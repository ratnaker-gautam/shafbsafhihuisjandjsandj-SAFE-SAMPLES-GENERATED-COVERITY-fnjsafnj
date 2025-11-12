//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

typedef void (*GraphOperation)(Graph*, int, int);

void initialize_graph(Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    g->node_count = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    if (weight < 0 || weight > 1000) {
        fprintf(stderr, "Invalid edge weight\n");
        exit(1);
    }
    g->adjacency[from][to] = weight;
}

void print_adjacency(Graph *g, int start, int unused) {
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    printf("Adjacency list for node %d:\n", start);
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[start][i] != 0) {
            printf("  -> %d (weight: %d)\n", i, g->adjacency[start][i]);
        }
    }
}

void find_neighbors(Graph *g, int node, int threshold) {
    if (node < 0 || node >= g->node_count) {
        fprintf(stderr, "Invalid node\n");
        return;
    }
    if (threshold < 0) {
        fprintf(stderr, "Invalid threshold\n");
        return;
    }
    printf("Neighbors of node %d with weight >= %d:\n", node, threshold);
    int found = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] >= threshold) {
            printf("  -> %d (weight: %d)\n", i, g->adjacency[node][i]);
            found = 1;
        }
    }
    if (!found) {
        printf("  No neighbors found\n");
    }
}

void traverse_graph(Graph *g, int start, int operation_type) {
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES];
    int top = 0;
    stack[top++] = start;
    visited[start] = 1;
    
    printf("Graph traversal from node %d:\n", start);
    while (top > 0) {
        int current = stack[--top];
        printf("Visited node %d\n", current);
        
        for (int i = 0; i < g->node_count; i++) {
            if (g->adjacency[current][i] != 0 && !visited[i]) {
                if (top >= MAX_NODES) {
                    fprintf(stderr, "Stack overflow\n");
                    return;
                }
                stack[top++] = i;
                visited[i] = 1;
            }
        }
    }
}

void execute_operation(Graph *g, GraphOperation op, int param1, int param2) {
    if (g == NULL || op == NULL) {
        fprintf(stderr, "Invalid operation parameters\n");
        return;
    }
    op(g, param1, param2);
}

int main() {
    Graph g;
    initialize_graph(&g, 6);
    
    add_edge(&g, 0, 1, 5);
    add_edge(&g, 0, 2, 3);
    add_edge(&g, 1, 3, 2);
    add_edge(&g, 2, 3, 7);
    add_edge(&g, 3, 4, 1);
    add_edge(&g, 4, 5, 4);
    add_edge(&g, 1, 4, 6);
    
    GraphOperation operations[] = {
        print_adjacency,
        find_neighbors,
        traverse_graph
    };
    
    int operation_count = sizeof(operations) / sizeof(operations[0]);
    
    printf("Graph Operations Demo\n");
    printf("====================\n\n");
    
    for (int i = 0; i < operation_count; i++) {
        switch (i) {
            case 0:
                printf("Operation 1: Print adjacency for node 0\n");
                execute_operation(&g, operations[i], 0, 0);
                break;
            case 1:
                printf("\nOperation 2: Find neighbors of node 1 with weight >= 3\n");
                execute_operation(&g, operations[i], 1, 3);
                break;
            case 2:
                printf("\nOperation 3: Traverse graph from node 0\n");
                execute_operation(&g