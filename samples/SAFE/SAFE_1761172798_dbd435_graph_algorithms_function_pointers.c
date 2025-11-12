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

void bfs_traversal(Graph *g, int start) {
    if (start < 0 || start >= g->node_count) return;
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS traversal: ");
    
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

void dfs_util(Graph *g, int node, int visited[]) {
    visited[node] = 1;
    printf("%d ", node);
    
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from == node) {
            int neighbor = g->edges[i].to;
            if (!visited[neighbor]) {
                dfs_util(g, neighbor, visited);
            }
        }
    }
}

void dfs_traversal(Graph *g, int start) {
    if (start < 0 || start >= g->node_count) return;
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
}

void print_adjacency_list(Graph *g, int dummy) {
    printf("Adjacency list:\n");
    for (int i = 0; i < g->node_count; i++) {
        printf("Node %d: ", i);
        int has_neighbors = 0;
        for (int j = 0; j < g->edge_count; j++) {
            if (g->edges[j].from == i) {
                printf("%d(%d) ", g->edges[j].to, g->edges[j].weight);
                has_neighbors = 1;
            }
        }
        if (!has_neighbors) printf("none");
        printf("\n");
    }
}

void execute_graph_operation(Graph *g, GraphOperation op, int param) {
    if (g && op) {
        op(g, param);
    }
}

int main() {
    Graph graph;
    initialize_graph(&graph);
    
    add_edge(&graph, 0, 1, 5);
    add_edge(&graph, 0, 2, 3);
    add_edge(&graph, 1, 3, 2);
    add_edge(&graph, 2, 3, 7);
    add_edge(&graph, 3, 4, 1);
    add_edge(&graph, 4, 0, 4);
    
    GraphOperation operations[] = {
        print_adjacency_list,
        bfs_traversal,
        dfs_traversal
    };
    
    int operation_count = sizeof(operations) / sizeof(operations[0]);
    
    for (int i = 0; i < operation_count; i++) {
        execute_graph_operation(&graph, operations[i], 0);
    }
    
    printf("Running BFS from different starting points:\n");
    for (int start = 0; start < graph.node_count && start < 3; start++) {
        execute_graph_operation(&graph, bfs_traversal, start);
    }
    
    return 0;
}