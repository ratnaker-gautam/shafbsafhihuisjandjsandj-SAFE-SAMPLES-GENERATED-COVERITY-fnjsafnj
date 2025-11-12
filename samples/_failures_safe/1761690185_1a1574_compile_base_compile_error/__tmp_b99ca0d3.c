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

typedef void (*GraphAlgorithm)(Graph*);

void validate_graph(Graph* g) {
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
            fprintf(stderr, "Invalid edge node indices\n");
            exit(1);
        }
    }
}

void bfs_traversal(Graph* g) {
    validate_graph(g);
    printf("BFS Traversal:\n");
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    queue[rear++] = 0;
    visited[0] = 1;
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == current && !visited[g->edges[i].to]) {
                if (rear >= MAX_NODES) {
                    fprintf(stderr, "Queue overflow\n");
                    return;
                }
                queue[rear++] = g->edges[i].to;
                visited[g->edges[i].to] = 1;
            }
        }
    }
    printf("\n");
}

void dfs_util(Graph* g, int node, int* visited) {
    printf("%d ", node);
    visited[node] = 1;
    
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from == node && !visited[g->edges[i].to]) {
            dfs_util(g, g->edges[i].to, visited);
        }
    }
}

void dfs_traversal(Graph* g) {
    validate_graph(g);
    printf("DFS Traversal:\n");
    
    int visited[MAX_NODES] = {0};
    dfs_util(g, 0, visited);
    printf("\n");
}

void find_connected_components(Graph* g) {
    validate_graph(g);
    printf("Connected Components:\n");
    
    int visited[MAX_NODES] = {0};
    int component_count = 0;
    
    for (int i = 0; i < g->node_count; i++) {
        if (!visited[i]) {
            printf("Component %d: ", ++component_count);
            int stack[MAX_NODES];
            int top = 0;
            
            stack[top++] = i;
            visited[i] = 1;
            
            while (top > 0) {
                int current = stack[--top];
                printf("%d ", current);
                
                for (int j = 0; j < g->edge_count; j++) {
                    if (g->edges[j].from == current && !visited[g->edges[j].to]) {
                        if (top >= MAX_NODES) {
                            fprintf(stderr, "Stack overflow\n");
                            return;
                        }
                        stack[top++] = g->edges[j].to;
                        visited[g->edges[j].to] = 1;
                    }
                    if (g->edges[j].to == current && !visited[g->edges[j].from]) {
                        if (top >= MAX_NODES) {
                            fprintf(stderr, "Stack overflow\n");
                            return;
                        }
                        stack[top++] = g->edges[j].from;
                        visited[g->edges[j].from] = 1;
                    }
                }
            }
            printf("\n");
        }
    }
}

void execute_algorithm(GraphAlgorithm algo, Graph* g) {
    if (algo == NULL) {
        fprintf(stderr, "Algorithm function is null\n");
        return;
    }
    algo(g);
}

Graph create_sample_graph() {
    Graph g;
    g.node_count = 6;
    g.edge_count = 7;
    
    Edge edges[] = {
        {0, 1, 1}, {0, 2, 1}, {1, 3, 1}, 
        {2, 3, 1}, {3, 4, 1}, {4, 5, 1},
        {1, 4, 1}
    };
    
    for (int i = 0; i < g.edge_count; i++) {
        g.edges[i] = edges[i];
    }
    
    return g;
}

int main() {
    Graph g = create_sample_graph();
    
    GraphAlgorithm algorithms[] = {
        bfs_traversal,
        dfs_t