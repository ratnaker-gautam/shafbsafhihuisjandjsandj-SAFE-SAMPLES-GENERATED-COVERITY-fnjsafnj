//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int num_nodes;
    int num_edges;
    int *adj_matrix;
};

void init_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = 0;
    size_t matrix_size = (size_t)nodes * (size_t)nodes * sizeof(int);
    if (matrix_size > SIZE_MAX / sizeof(int)) {
        fprintf(stderr, "Matrix size too large\n");
        exit(1);
    }
    g->adj_matrix = malloc(matrix_size);
    if (g->adj_matrix == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    memset(g->adj_matrix, 0, matrix_size);
}

void add_edge(struct Graph *g, int u, int v) {
    if (g == NULL || g->adj_matrix == NULL) return;
    if (u < 0 || u >= g->num_nodes || v < 0 || v >= g->num_nodes) {
        fprintf(stderr, "Invalid node index\n");
        return;
    }
    if (u == v) return;
    
    int *matrix = g->adj_matrix;
    *(matrix + u * g->num_nodes + v) = 1;
    *(matrix + v * g->num_nodes + u) = 1;
    g->num_edges++;
}

void dfs_util(struct Graph *g, int node, int *visited) {
    if (visited == NULL) return;
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
    if (g == NULL || g->adj_matrix == NULL) return;
    if (start < 0 || start >= g->num_nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int *visited = calloc((size_t)g->num_nodes, sizeof(int));
    if (visited == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
    
    free(visited);
}

void bfs(struct Graph *g, int start) {
    if (g == NULL || g->adj_matrix == NULL) return;
    if (start < 0 || start >= g->num_nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int *visited = calloc((size_t)g->num_nodes, sizeof(int));
    if (visited == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    
    int *queue = malloc((size_t)g->num_nodes * sizeof(int));
    if (queue == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
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
                if (rear >= g->num_nodes) {
                    fprintf(stderr, "Queue overflow\n");
                    free(visited);
                    free(queue);
                    return;
                }
            }
        }
    }
    printf("\n");
    
    free(visited);
    free(queue);
}

void free_graph(struct Graph *g) {
    if (g != NULL) {
        free(g->adj_matrix);
        g->adj_matrix = NULL;
        g->num_nodes = 0;
        g->num_edges = 0;
    }
}

int main() {
    struct Graph g;
    int choice, u, v, start;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &g.num_nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (g.num_nodes <= 0 || g.num_nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    
    init_graph(&g, g.num_nodes);
    
    while (1) {
        printf("\n1. Add edge\n2. DFS\n3. BFS\n4. Exit\nChoice: ");
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Invalid input\n");
            break