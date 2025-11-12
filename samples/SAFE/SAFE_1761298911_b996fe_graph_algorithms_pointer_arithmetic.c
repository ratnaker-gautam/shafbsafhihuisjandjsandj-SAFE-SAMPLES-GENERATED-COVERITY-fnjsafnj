//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int num_nodes;
    int num_edges;
    int *adj_matrix;
};

struct Graph* create_graph(int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) return NULL;
    
    struct Graph *g = malloc(sizeof(struct Graph));
    if (!g) return NULL;
    
    g->num_nodes = nodes;
    g->num_edges = 0;
    g->adj_matrix = calloc(nodes * nodes, sizeof(int));
    if (!g->adj_matrix) {
        free(g);
        return NULL;
    }
    
    return g;
}

void destroy_graph(struct Graph *g) {
    if (g) {
        free(g->adj_matrix);
        free(g);
    }
}

int add_edge(struct Graph *g, int u, int v) {
    if (!g || u < 0 || v < 0 || u >= g->num_nodes || v >= g->num_nodes || u == v) return 0;
    
    int *matrix = g->adj_matrix;
    if (*(matrix + u * g->num_nodes + v) == 0) {
        *(matrix + u * g->num_nodes + v) = 1;
        *(matrix + v * g->num_nodes + u) = 1;
        g->num_edges++;
        return 1;
    }
    return 0;
}

void dfs_util(struct Graph *g, int node, int *visited) {
    if (!g || !visited || node < 0 || node >= g->num_nodes) return;
    
    *(visited + node) = 1;
    printf("%d ", node);
    
    int *matrix = g->adj_matrix;
    for (int i = 0; i < g->num_nodes; i++) {
        if (*(matrix + node * g->num_nodes + i) && !*(visited + i)) {
            dfs_util(g, i, visited);
        }
    }
}

void dfs(struct Graph *g, int start) {
    if (!g || start < 0 || start >= g->num_nodes) return;
    
    int *visited = calloc(g->num_nodes, sizeof(int));
    if (!visited) return;
    
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
    
    free(visited);
}

void bfs(struct Graph *g, int start) {
    if (!g || start < 0 || start >= g->num_nodes) return;
    
    int *visited = calloc(g->num_nodes, sizeof(int));
    if (!visited) return;
    
    int *queue = malloc(g->num_nodes * sizeof(int));
    if (!queue) {
        free(visited);
        return;
    }
    
    int front = 0, rear = 0;
    *(visited + start) = 1;
    *(queue + rear) = start;
    rear++;
    
    printf("BFS traversal: ");
    
    while (front < rear) {
        int current = *(queue + front);
        front++;
        printf("%d ", current);
        
        int *matrix = g->adj_matrix;
        for (int i = 0; i < g->num_nodes; i++) {
            if (*(matrix + current * g->num_nodes + i) && !*(visited + i)) {
                *(visited + i) = 1;
                *(queue + rear) = i;
                rear++;
            }
        }
    }
    printf("\n");
    
    free(queue);
    free(visited);
}

int main() {
    struct Graph *g = create_graph(6);
    if (!g) {
        printf("Failed to create graph\n");
        return 1;
    }
    
    add_edge(g, 0, 1);
    add_edge(g, 0, 2);
    add_edge(g, 1, 3);
    add_edge(g, 1, 4);
    add_edge(g, 2, 4);
    add_edge(g, 3, 5);
    add_edge(g, 4, 5);
    
    printf("Graph with %d nodes and %d edges\n", g->num_nodes, g->num_edges);
    
    dfs(g, 0);
    bfs(g, 0);
    
    destroy_graph(g);
    return 0;
}