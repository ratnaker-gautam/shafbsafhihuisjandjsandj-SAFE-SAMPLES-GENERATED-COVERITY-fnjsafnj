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

void dfs_util(Graph *g, int node, int visited[]) {
    if (node < 0 || node >= g->node_count) return;
    
    visited[node] = 1;
    printf("%d ", node);
    
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] && !visited[i]) {
            dfs_util(g, i, visited);
        }
    }
}

void depth_first_search(Graph *g, int start, int unused) {
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
}

void breadth_first_search(Graph *g, int start, int unused) {
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
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

void find_connected_components(Graph *g, int unused1, int unused2) {
    int visited[MAX_NODES] = {0};
    int component_count = 0;
    
    for (int i = 0; i < g->node_count; i++) {
        if (!visited[i]) {
            component_count++;
            printf("Component %d: ", component_count);
            
            int stack[MAX_NODES];
            int top = 0;
            stack[top++] = i;
            visited[i] = 1;
            
            while (top > 0) {
                int current = stack[--top];
                printf("%d ", current);
                
                for (int j = 0; j < g->node_count; j++) {
                    if (g->adjacency[current][j] && !visited[j]) {
                        visited[j] = 1;
                        if (top < MAX_NODES) {
                            stack[top++] = j;
                        }
                    }
                }
            }
            printf("\n");
        }
    }
    
    if (component_count == 0) {
        printf("No connected components found\n");
    }
}

void execute_graph_operation(Graph *g, GraphOperation op, int param1, int param2) {
    if (g == NULL || op == NULL) {
        fprintf(stderr, "Invalid operation parameters\n");
        return;
    }
    op(g, param1, param2);
}

int main() {
    Graph graph;
    int node_count = 6;
    
    initialize_graph(&graph, node_count);
    
    add_edge(&graph, 0, 1);
    add_edge(&graph, 0, 2);
    add_edge(&graph, 1, 3);
    add_edge(&graph, 2, 4);
    add_edge(&graph, 3, 5);
    add_edge(&graph, 4, 5);
    
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
    
    int operation_count = sizeof(operations) / sizeof(operations[0]);
    
    for (int i = 0; i < operation_count; i++) {
        printf("Operation %d: %s\n", i + 1, operation_names[i]);
        execute_graph_operation(&graph, operations[i], 0, 0);