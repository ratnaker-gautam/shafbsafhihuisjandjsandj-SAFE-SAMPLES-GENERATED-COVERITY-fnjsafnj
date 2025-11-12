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

void add_edge(Graph *g, int from, int to) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid edge\n");
        exit(1);
    }
    g->adjacency[from][to] = 1;
    g->adjacency[to][from] = 1;
}

void dfs_visit(Graph *g, int node, int *visited) {
    if (node < 0 || node >= g->node_count) return;
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] && !visited[i]) {
            dfs_visit(g, i, visited);
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
    dfs_visit(g, start, visited);
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
                } else {
                    fprintf(stderr, "Queue overflow\n");
                    return;
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
            printf("Component %d: ", ++component_count);
            int stack[MAX_NODES];
            int top = 0;
            
            visited[i] = 1;
            stack[top++] = i;
            
            while (top > 0) {
                int current = stack[--top];
                printf("%d ", current);
                
                for (int j = 0; j < g->node_count; j++) {
                    if (g->adjacency[current][j] && !visited[j]) {
                        visited[j] = 1;
                        if (top < MAX_NODES) {
                            stack[top++] = j;
                        } else {
                            fprintf(stderr, "Stack overflow\n");
                            return;
                        }
                    }
                }
            }
            printf("\n");
        }
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
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
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
    
    if (edges < 0 || edges > nodes * (nodes - 1) / 2) {
        fprintf(stderr, "Invalid edge count\n");
        return 1;
    }
    
    printf("Enter edges (from to):\n");
    for (int i = 0; i < edges; i++) {
        int from, to;
        if (scanf("%d %d", &from, &to) != 2) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&graph