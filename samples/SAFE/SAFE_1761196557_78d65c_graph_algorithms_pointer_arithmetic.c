//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 500

struct Graph {
    int num_nodes;
    int num_edges;
    int *adj_matrix;
};

int graph_init(struct Graph *g, int nodes) {
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES) return -1;
    
    g->num_nodes = nodes;
    g->num_edges = 0;
    size_t matrix_size = (size_t)nodes * (size_t)nodes * sizeof(int);
    if (matrix_size / sizeof(int) != (size_t)(nodes * nodes)) return -1;
    
    g->adj_matrix = malloc(matrix_size);
    if (g->adj_matrix == NULL) return -1;
    
    memset(g->adj_matrix, 0, matrix_size);
    return 0;
}

void graph_free(struct Graph *g) {
    if (g != NULL) {
        free(g->adj_matrix);
        g->adj_matrix = NULL;
    }
}

int graph_add_edge(struct Graph *g, int from, int to, int weight) {
    if (g == NULL || from < 0 || to < 0 || from >= g->num_nodes || to >= g->num_nodes) return -1;
    if (weight < 0) return -1;
    
    int *matrix = g->adj_matrix;
    *(matrix + from * g->num_nodes + to) = weight;
    *(matrix + to * g->num_nodes + from) = weight;
    g->num_edges++;
    return 0;
}

void graph_dfs(const struct Graph *g, int start, int *visited) {
    if (g == NULL || visited == NULL || start < 0 || start >= g->num_nodes) return;
    
    visited[start] = 1;
    printf("Visited node %d\n", start);
    
    int *matrix = g->adj_matrix;
    for (int i = 0; i < g->num_nodes; i++) {
        int has_edge = *(matrix + start * g->num_nodes + i);
        if (has_edge > 0 && !visited[i]) {
            graph_dfs(g, i, visited);
        }
    }
}

void graph_bfs(const struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->num_nodes) return;
    
    int *visited = calloc((size_t)g->num_nodes, sizeof(int));
    if (visited == NULL) return;
    
    int *queue = malloc((size_t)g->num_nodes * sizeof(int));
    if (queue == NULL) {
        free(visited);
        return;
    }
    
    int front = 0, rear = 0;
    visited[start] = 1;
    *(queue + rear) = start;
    rear++;
    
    printf("BFS traversal from node %d:\n", start);
    
    while (front < rear) {
        int current = *(queue + front);
        front++;
        printf("Visited node %d\n", current);
        
        int *matrix = g->adj_matrix;
        for (int i = 0; i < g->num_nodes; i++) {
            int has_edge = *(matrix + current * g->num_nodes + i);
            if (has_edge > 0 && !visited[i]) {
                visited[i] = 1;
                if (rear < g->num_nodes) {
                    *(queue + rear) = i;
                    rear++;
                }
            }
        }
    }
    
    free(visited);
    free(queue);
}

int main() {
    struct Graph g;
    int num_nodes;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1 || num_nodes <= 0 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    if (graph_init(&g, num_nodes) != 0) {
        printf("Failed to initialize graph\n");
        return 1;
    }
    
    printf("Graph created with %d nodes\n", num_nodes);
    printf("Adding sample edges...\n");
    
    for (int i = 0; i < num_nodes - 1; i++) {
        if (graph_add_edge(&g, i, i + 1, 1) != 0) {
            printf("Failed to add edge\n");
            graph_free(&g);
            return 1;
        }
    }
    
    if (num_nodes > 2) {
        if (graph_add_edge(&g, 0, num_nodes - 1, 1) != 0) {
            printf("Failed to add edge\n");
            graph_free(&g);
            return 1;
        }
    }
    
    printf("Graph has %d edges\n", g.num_edges);
    
    int *visited = calloc((size_t)num_nodes, sizeof(int));
    if (visited == NULL) {
        printf("Memory allocation failed\n");
        graph_free(&g);
        return 1;
    }
    
    printf("\nDFS traversal:\n");
    graph_dfs(&g, 0, visited);
    
    printf("\n");
    graph_bfs(&g, 0);
    
    free(visited);
    graph_free(&g);
    
    return 0;
}