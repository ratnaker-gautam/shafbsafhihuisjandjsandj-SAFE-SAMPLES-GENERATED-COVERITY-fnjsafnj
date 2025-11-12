//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

typedef struct {
    int from;
    int to;
    int weight;
} Edge;

typedef struct {
    int node_count;
    int edge_count;
    Edge edges[MAX_EDGES];
} Graph;

typedef void (*GraphOperation)(Graph*, int);

void initialize_graph(Graph *g) {
    g->node_count = 0;
    g->edge_count = 0;
    memset(g->edges, 0, sizeof(g->edges));
}

int add_edge(Graph *g, int from, int to, int weight) {
    if (g == NULL || from < 0 || to < 0 || from >= MAX_NODES || to >= MAX_NODES) {
        return 0;
    }
    if (g->edge_count >= MAX_EDGES) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    
    g->edges[g->edge_count].from = from;
    g->edges[g->edge_count].to = to;
    g->edges[g->edge_count].weight = weight;
    g->edge_count++;
    
    if (from >= g->node_count) g->node_count = from + 1;
    if (to >= g->node_count) g->node_count = to + 1;
    
    return 1;
}

void bfs_traversal(Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->node_count) {
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS traversal from node %d: ", start);
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == current) {
                int neighbor = g->edges[i].to;
                if (!visited[neighbor]) {
                    visited[neighbor] = 1;
                    if (rear < MAX_NODES) {
                        queue[rear++] = neighbor;
                    }
                }
            }
        }
    }
    printf("\n");
}

void dfs_traversal(Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->node_count) {
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES];
    int top = 0;
    
    visited[start] = 1;
    stack[top++] = start;
    
    printf("DFS traversal from node %d: ", start);
    
    while (top > 0) {
        int current = stack[--top];
        printf("%d ", current);
        
        for (int i = g->edge_count - 1; i >= 0; i--) {
            if (g->edges[i].from == current) {
                int neighbor = g->edges[i].to;
                if (!visited[neighbor]) {
                    visited[neighbor] = 1;
                    if (top < MAX_NODES) {
                        stack[top++] = neighbor;
                    }
                }
            }
        }
    }
    printf("\n");
}

void find_neighbors(Graph *g, int node) {
    if (g == NULL || node < 0 || node >= g->node_count) {
        return;
    }
    
    printf("Neighbors of node %d: ", node);
    int found = 0;
    
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from == node) {
            printf("%d ", g->edges[i].to);
            found = 1;
        }
    }
    
    if (!found) {
        printf("none");
    }
    printf("\n");
}

void execute_graph_operation(Graph *g, GraphOperation operation, int start_node) {
    if (g == NULL || operation == NULL) {
        return;
    }
    operation(g, start_node);
}

int main() {
    Graph graph;
    initialize_graph(&graph);
    
    add_edge(&graph, 0, 1, 1);
    add_edge(&graph, 0, 2, 1);
    add_edge(&graph, 1, 3, 1);
    add_edge(&graph, 2, 3, 1);
    add_edge(&graph, 3, 4, 1);
    add_edge(&graph, 4, 5, 1);
    
    printf("Graph operations:\n");
    
    execute_graph_operation(&graph, bfs_traversal, 0);
    execute_graph_operation(&graph, dfs_traversal, 0);
    execute_graph_operation(&graph, find_neighbors, 3);
    execute_graph_operation(&graph, find_neighbors, 5);
    
    return 0;
}