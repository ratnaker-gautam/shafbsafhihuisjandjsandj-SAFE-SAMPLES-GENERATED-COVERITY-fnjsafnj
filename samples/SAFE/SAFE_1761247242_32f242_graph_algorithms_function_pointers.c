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

void initialize_graph(Graph *g, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int from, int to) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid edge\n");
        exit(1);
    }
    g->adjacency[from][to] = 1;
    g->adjacency[to][from] = 1;
}

void depth_first_traversal(Graph *g, int start, int visited[]) {
    if (start < 0 || start >= g->node_count) return;
    visited[start] = 1;
    printf("%d ", start);
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[start][i] && !visited[i]) {
            depth_first_traversal(g, i, visited);
        }
    }
}

void breadth_first_traversal(Graph *g, int start, int visited[]) {
    if (start < 0 || start >= g->node_count) return;
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        for (int i = 0; i < g->node_count; i++) {
            if (g->adjacency[current][i] && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
}

void perform_operation(Graph *g, int start, GraphOperation op) {
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    op(g, start, visited);
    printf("\n");
}

void print_graph(Graph *g, int unused1, int unused2) {
    printf("Adjacency matrix:\n");
    for (int i = 0; i < g->node_count; i++) {
        for (int j = 0; j < g->node_count; j++) {
            printf("%d ", g->adjacency[i][j]);
        }
        printf("\n");
    }
}

int main() {
    Graph g;
    int node_count, edge_count;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1 || node_count <= 0 || node_count > MAX_NODES) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initialize_graph(&g, node_count);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1 || edge_count < 0) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Enter edges (from to):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to;
        if (scanf("%d %d", &from, &to) != 2) {
            fprintf(stderr, "Invalid input\n");
            return 1;
        }
        add_edge(&g, from, to);
    }
    
    GraphOperation operations[] = {
        print_graph,
        depth_first_traversal,
        breadth_first_traversal
    };
    const char* operation_names[] = {
        "Print graph",
        "Depth-first traversal",
        "Breadth-first traversal"
    };
    int operation_count = sizeof(operations) / sizeof(operations[0]);
    
    int choice;
    printf("\nChoose operation:\n");
    for (int i = 0; i < operation_count; i++) {
        printf("%d. %s\n", i, operation_names[i]);
    }
    
    if (scanf("%d", &choice) != 1 || choice < 0 || choice >= operation_count) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    if (choice != 0) {
        int start_node;
        printf("Enter start node: ");
        if (scanf("%d", &start_node) != 1) {
            fprintf(stderr, "Invalid input\n");
            return 1;
        }
        perform_operation(&g, start_node, operations[choice]);
    } else {
        perform_operation(&g, 0, operations[choice]);
    }
    
    return 0;
}