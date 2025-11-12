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

void validate_graph(Graph *g) {
    if (g == NULL) {
        fprintf(stderr, "Graph is NULL\n");
        exit(1);
    }
    if (g->node_count <= 0 || g->node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    if (g->edge_count < 0 || g->edge_count > MAX_EDGES) {
        fprintf(stderr, "Invalid edge count\n");
        exit(1);
    }
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from < 0 || g->edges[i].from >= g->node_count ||
            g->edges[i].to < 0 || g->edges[i].to >= g->node_count) {
            fprintf(stderr, "Invalid edge node indices\n");
            exit(1);
        }
    }
}

void bfs_traversal(Graph *g, int start) {
    validate_graph(g);
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
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
                    } else {
                        fprintf(stderr, "Queue overflow\n");
                        return;
                    }
                }
            }
        }
    }
    printf("\n");
}

void dfs_traversal(Graph *g, int start) {
    validate_graph(g);
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES];
    int top = 0;
    
    stack[top++] = start;
    printf("DFS traversal from node %d: ", start);
    
    while (top > 0) {
        int current = stack[--top];
        
        if (!visited[current]) {
            visited[current] = 1;
            printf("%d ", current);
            
            for (int i = g->edge_count - 1; i >= 0; i--) {
                if (g->edges[i].from == current) {
                    int neighbor = g->edges[i].to;
                    if (!visited[neighbor]) {
                        if (top < MAX_NODES) {
                            stack[top++] = neighbor;
                        } else {
                            fprintf(stderr, "Stack overflow\n");
                            return;
                        }
                    }
                }
            }
        }
    }
    printf("\n");
}

void print_adjacency_list(Graph *g, int unused) {
    validate_graph(g);
    printf("Adjacency list:\n");
    for (int i = 0; i < g->node_count; i++) {
        printf("Node %d: ", i);
        int neighbor_count = 0;
        for (int j = 0; j < g->edge_count; j++) {
            if (g->edges[j].from == i) {
                if (neighbor_count > 0) {
                    printf(" ");
                }
                printf("%d", g->edges[j].to);
                neighbor_count++;
            }
        }
        printf("\n");
    }
}

void execute_operation(Graph *g, GraphOperation op, int param) {
    if (op == NULL) {
        fprintf(stderr, "Operation is NULL\n");
        return;
    }
    op(g, param);
}

Graph create_sample_graph() {
    Graph g;
    g.node_count = 6;
    g.edge_count = 8;
    
    Edge edges[] = {
        {0, 1, 1}, {0, 2, 1}, {1, 3, 1}, {1, 4, 1},
        {2, 4, 1}, {3, 5, 1}, {4, 5, 1}, {5, 0, 1}
    };
    
    for (int i = 0; i < g.edge_count; i++) {
        if (i < MAX_EDGES) {
            g.edges[i] = edges[i];
        }
    }
    
    return g;
}

int main() {
    Graph g = create_sample_graph();
    GraphOperation operations[] = {bfs_traversal, dfs_traversal, print_adjacency_list};
    const char* operation_names[] = {"B