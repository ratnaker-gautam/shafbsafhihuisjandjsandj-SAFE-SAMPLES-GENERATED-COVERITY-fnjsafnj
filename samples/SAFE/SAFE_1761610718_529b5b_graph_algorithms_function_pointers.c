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
}

int add_edge(Graph *g, int from, int to, int weight) {
    if (g->edge_count >= MAX_EDGES) return 0;
    if (from < 0 || from >= MAX_NODES) return 0;
    if (to < 0 || to >= MAX_NODES) return 0;
    
    g->edges[g->edge_count].from = from;
    g->edges[g->edge_count].to = to;
    g->edges[g->edge_count].weight = weight;
    g->edge_count++;
    
    if (from >= g->node_count) g->node_count = from + 1;
    if (to >= g->node_count) g->node_count = to + 1;
    
    return 1;
}

void dfs_visit(Graph *g, int node, int *visited) {
    if (node < 0 || node >= g->node_count) return;
    if (visited[node]) return;
    
    visited[node] = 1;
    printf("%d ", node);
    
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from == node) {
            dfs_visit(g, g->edges[i].to, visited);
        }
    }
}

void depth_first_search(Graph *g, int start) {
    if (start < 0 || start >= g->node_count) return;
    
    int visited[MAX_NODES] = {0};
    printf("DFS from %d: ", start);
    dfs_visit(g, start, visited);
    printf("\n");
}

void breadth_first_search(Graph *g, int start) {
    if (start < 0 || start >= g->node_count) return;
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS from %d: ", start);
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == current) {
                int neighbor = g->edges[i].to;
                if (!visited[neighbor]) {
                    visited[neighbor] = 1;
                    queue[rear++] = neighbor;
                }
            }
        }
    }
    printf("\n");
}

void print_graph(Graph *g, int dummy) {
    printf("Graph (%d nodes, %d edges):\n", g->node_count, g->edge_count);
    for (int i = 0; i < g->edge_count; i++) {
        printf("  %d -> %d (weight: %d)\n", 
               g->edges[i].from, g->edges[i].to, g->edges[i].weight);
    }
}

void execute_operation(Graph *g, GraphOperation op, int param) {
    if (g && op) {
        op(g, param);
    }
}

int main() {
    Graph g;
    initialize_graph(&g);
    
    add_edge(&g, 0, 1, 1);
    add_edge(&g, 0, 2, 1);
    add_edge(&g, 1, 3, 1);
    add_edge(&g, 2, 3, 1);
    add_edge(&g, 3, 4, 1);
    add_edge(&g, 4, 0, 1);
    
    GraphOperation operations[] = {
        print_graph,
        depth_first_search,
        breadth_first_search
    };
    
    int operation_count = sizeof(operations) / sizeof(operations[0]);
    
    for (int i = 0; i < operation_count; i++) {
        execute_operation(&g, operations[i], 0);
    }
    
    for (int start = 0; start < 3; start++) {
        execute_operation(&g, depth_first_search, start);
        execute_operation(&g, breadth_first_search, start);
    }
    
    return 0;
}