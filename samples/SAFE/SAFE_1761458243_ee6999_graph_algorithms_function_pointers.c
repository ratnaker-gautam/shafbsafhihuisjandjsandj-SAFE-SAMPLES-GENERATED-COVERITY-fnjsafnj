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

int validate_node(Graph *g, int node) {
    return (g != NULL && node >= 0 && node < g->node_count);
}

void add_edge(Graph *g, int from, int to) {
    if (!validate_node(g, from) || !validate_node(g, to)) {
        return;
    }
    g->adjacency[from][to] = 1;
    g->adjacency[to][from] = 1;
}

void dfs_util(Graph *g, int node, int visited[]) {
    if (g == NULL || visited == NULL || !validate_node(g, node)) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] && !visited[i]) {
            dfs_util(g, i, visited);
        }
    }
}

void depth_first_search(Graph *g, int start, int unused) {
    if (g == NULL || !validate_node(g, start)) {
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
}

void breadth_first_search(Graph *g, int start, int unused) {
    if (g == NULL || !validate_node(g, start)) {
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

void count_components(Graph *g, int unused1, int unused2) {
    if (g == NULL) {
        return;
    }
    int visited[MAX_NODES] = {0};
    int component_count = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (!visited[i]) {
            component_count++;
            int stack[MAX_NODES];
            int top = 0;
            stack[top++] = i;
            visited[i] = 1;
            while (top > 0) {
                int current = stack[--top];
                for (int j = 0; j < g->node_count; j++) {
                    if (g->adjacency[current][j] && !visited[j]) {
                        visited[j] = 1;
                        if (top < MAX_NODES) {
                            stack[top++] = j;
                        }
                    }
                }
            }
        }
    }
    printf("Number of connected components: %d\n", component_count);
}

void execute_operation(Graph *g, GraphOperation op, int param1, int param2) {
    if (g == NULL || op == NULL) {
        return;
    }
    op(g, param1, param2);
}

int main() {
    Graph g;
    initialize_graph(&g, 6);
    add_edge(&g, 0, 1);
    add_edge(&g, 0, 2);
    add_edge(&g, 1, 2);
    add_edge(&g, 3, 4);
    
    GraphOperation operations[] = {
        depth_first_search,
        breadth_first_search,
        count_components
    };
    
    int operation_count = sizeof(operations) / sizeof(operations[0]);
    
    for (int i = 0; i < operation_count; i++) {
        execute_operation(&g, operations[i], 0, 0);
    }
    
    return 0;
}