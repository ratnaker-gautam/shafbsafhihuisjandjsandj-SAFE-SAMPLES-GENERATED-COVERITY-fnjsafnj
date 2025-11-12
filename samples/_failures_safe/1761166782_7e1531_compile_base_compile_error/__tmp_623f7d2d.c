//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
c
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
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES) return;
    g->node_count = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

int validate_node(Graph *g, int node) {
    return (g != NULL && node >= 0 && node < g->node_count);
}

void add_edge(Graph *g, int from, int to) {
    if (!validate_node(g, from) || !validate_node(g, to)) return;
    g->adjacency[from][to] = 1;
    g->adjacency[to][from] = 1;
}

void dfs_visit(Graph *g, int node, int *visited) {
    if (g == NULL || visited == NULL || !validate_node(g, node)) return;
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] && !visited[i]) {
            dfs_visit(g, i, visited);
        }
    }
}

void depth_first_search(Graph *g, int start, int unused) {
    if (g == NULL || !validate_node(g, start)) return;
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_visit(g, start, visited);
    printf("\n");
}

void breadth_first_search(Graph *g, int start, int unused) {
    if (g == NULL || !validate_node(g, start)) return;
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

void find_connected_components(Graph *g, int unused1, int unused2) {
    if (g == NULL) return;
    int visited[MAX_NODES] = {0};
    int component_count = 0;
    
    for (int i = 0; i < g->node_count; i++) {
        if (!visited[i]) {
            printf("Component %d: ", ++component_count);
            dfs_visit(g, i, visited);
            printf("\n");
        }
    }
}

void execute_graph_operation(Graph *g, GraphOperation op, int param1, int param2) {
    if (g == NULL || op == NULL) return;
    op(g, param1, param2);
}

int main() {
    Graph g;
    int node_count, edge_count;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1 || node_count <= 0 || node_count > MAX_NODES) {
        printf("Invalid node count\n");
        return 1;
    }
    
    initialize_graph(&g, node_count);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1 || edge_count < 0) {
        printf("Invalid edge count\n");
        return 1;
    }
    
    printf("Enter edges (from to):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to;
        if (scanf("%d %d", &from, &to) != 2) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (from < 0 || from >= node_count || to < 0 || to >= node_count) {
            printf("Invalid node index\n");
            return 1;
        }
        add_edge(&g, from, to);
    }
    
    GraphOperation operations[] = {
        depth_first_search,
        breadth_first_search,
        find_connected_components
    };
    const char* operation_names[] = {
        "Depth First Search",
        "Breadth First Search", 
        "Find Connected Components"
    };
    int op_count = sizeof(operations) / sizeof(operations[0]);
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < op_count; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    int choice;
    printf("Select operation (1-%d): ", op_count);
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > op_count) {
        printf("Invalid