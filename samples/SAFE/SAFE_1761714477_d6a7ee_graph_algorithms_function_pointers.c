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
        fprintf(stderr, "Graph is null\n");
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
            fprintf(stderr, "Invalid edge endpoints\n");
            exit(1);
        }
    }
}

void dfs_visit(Graph *g, int node, int *visited) {
    if (visited == NULL) return;
    if (node < 0 || node >= g->node_count) return;
    if (visited[node]) return;
    
    visited[node] = 1;
    printf("%d ", node);
    
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from == node && !visited[g->edges[i].to]) {
            dfs_visit(g, g->edges[i].to, visited);
        }
    }
}

void depth_first_search(Graph *g, int start) {
    validate_graph(g);
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    printf("DFS from node %d: ", start);
    dfs_visit(g, start, visited);
    printf("\n");
}

void breadth_first_search(Graph *g, int start) {
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
    
    printf("BFS from node %d: ", start);
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == current && !visited[g->edges[i].to]) {
                if (rear >= MAX_NODES) {
                    fprintf(stderr, "Queue overflow\n");
                    return;
                }
                visited[g->edges[i].to] = 1;
                queue[rear++] = g->edges[i].to;
            }
        }
    }
    printf("\n");
}

void find_connected_components(Graph *g, int unused) {
    validate_graph(g);
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

void perform_graph_operation(Graph *g, GraphOperation op, int start) {
    if (op == NULL) {
        fprintf(stderr, "Operation is null\n");
        return;
    }
    op(g, start);
}

Graph create_sample_graph() {
    Graph g;
    g.node_count = 6;
    g.edge_count = 7;
    
    Edge edges[] = {
        {0, 1, 1}, {0, 2, 1}, {1, 3, 1}, 
        {2, 3, 1}, {3, 4, 1}, {4, 5, 1}, {1, 5, 1}
    };
    
    for (int i = 0; i < g.edge_count; i++) {
        g.edges[i] = edges[i];
    }
    
    return g;
}

int main() {
    Graph g = create_sample_graph();
    GraphOperation operations[] = {
        depth_first_search,
        breadth_first_search,
        find_connected_components
    };
    
    int starts[] = {0, 0, 0};
    int op_count = sizeof(operations) / sizeof(operations[0]);
    
    for (int i = 0; i < op_count; i++) {
        perform_graph_operation(&g, operations[i], starts[i]);
    }
    
    return 0;
}