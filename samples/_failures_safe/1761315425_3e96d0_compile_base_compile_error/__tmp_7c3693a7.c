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
        exit(EXIT_FAILURE);
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
        fprintf(stderr, "Invalid edge parameters\n");
        return;
    }
    g->adjacency[from][to] = 1;
    g->adjacency[to][from] = 1;
}

void dfs_visit(Graph *g, int node, int *visited) {
    if (node < 0 || node >= g->node_count || visited == NULL) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] && !visited[i]) {
            dfs_visit(g, i, visited);
        }
    }
}

void depth_first_search(Graph *g, int start, int unused) {
    if (g == NULL || start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid DFS parameters\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_visit(g, start, visited);
    printf("\n");
}

void breadth_first_search(Graph *g, int start, int unused) {
    if (g == NULL || start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid BFS parameters\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    printf("BFS traversal: ");
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
    printf("\n");
}

void print_adjacency_matrix(Graph *g, int unused1, int unused2) {
    if (g == NULL) {
        fprintf(stderr, "Invalid graph\n");
        return;
    }
    printf("Adjacency matrix:\n");
    for (int i = 0; i < g->node_count; i++) {
        for (int j = 0; j < g->node_count; j++) {
            printf("%d ", g->adjacency[i][j]);
        }
        printf("\n");
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
    int node_count;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    initialize_graph(&g, node_count);
    
    int edge_count;
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter edges (from to):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to;
        if (scanf("%d %d", &from, &to) != 2) {
            fprintf(stderr, "Invalid edge input\n");
            return EXIT_FAILURE;
        }
        add_edge(&g, from, to);
    }
    
    GraphOperation operations[] = {
        print_adjacency_matrix,
        depth_first_search,
        breadth_first_search
    };
    const char* operation_names[] = {
        "Print adjacency matrix",
        "Depth-first search",
        "Breadth-first search"
    };
    int operation_count = sizeof(operations) / sizeof(operations[0]);
    
    while (1) {
        printf("\nAvailable operations:\n");
        for (int i = 0; i < operation_count; i++) {
            printf("%d. %s\n", i, operation_names[i]);
        }
        printf("%d. Exit\n", operation_count);
        
        int choice;