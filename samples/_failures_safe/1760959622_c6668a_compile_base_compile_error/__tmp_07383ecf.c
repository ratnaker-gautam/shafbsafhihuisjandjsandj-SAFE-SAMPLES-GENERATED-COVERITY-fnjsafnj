//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
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
    if (g == NULL || node_count <= 0 || node_count > MAX_NODES) {
        return;
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int from, int to) {
    if (g == NULL || from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        return;
    }
    g->adjacency[from][to] = 1;
}

void dfs_visit(Graph *g, int node, int *visited) {
    if (g == NULL || visited == NULL || node < 0 || node >= g->node_count) {
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
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_visit(g, start, visited);
    printf("\n");
}

void breadth_first_search(Graph *g, int start, int unused) {
    if (g == NULL || start < 0 || start >= g->node_count) {
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

void has_path(Graph *g, int start, int end) {
    if (g == NULL || start < 0 || start >= g->node_count || end < 0 || end >= g->node_count) {
        printf("Invalid nodes\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        if (current == end) {
            printf("Path exists from %d to %d\n", start, end);
            return;
        }
        for (int i = 0; i < g->node_count; i++) {
            if (g->adjacency[current][i] && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
    printf("No path from %d to %d\n", start, end);
}

void execute_operation(Graph *g, GraphOperation op, int param1, int param2) {
    if (g == NULL || op == NULL) {
        return;
    }
    op(g, param1, param2);
}

int main() {
    Graph g;
    int node_count;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1 || node_count <= 0 || node_count > MAX_NODES) {
        printf("Invalid input\n");
        return 1;
    }
    
    initialize_graph(&g, node_count);
    
    int edge_count;
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1 || edge_count < 0) {
        printf("Invalid input\n");
        return 1;
    }
    
    for (int i = 0; i < edge_count; i++) {
        int from, to;
        printf("Enter edge %d (from to): ", i + 1);
        if (scanf("%d %d", &from, &to) != 2) {
            printf("Invalid input\n");
            return 1;
        }
        if (from < 0 || from >= node_count || to < 0 || to >= node_count) {
            printf("Invalid node indices\n");
            return 1;
        }
        add_edge(&g, from, to);
    }
    
    GraphOperation operations[] = {depth_first_search, breadth_first_search, has_path};
    const char* operation_names