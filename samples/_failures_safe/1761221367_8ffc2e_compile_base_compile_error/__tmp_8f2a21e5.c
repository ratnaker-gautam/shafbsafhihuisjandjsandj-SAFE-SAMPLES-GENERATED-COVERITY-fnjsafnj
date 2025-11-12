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
    int edges[MAX_EDGES][2];
};

void init_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = 0;
    g->adj_matrix = calloc(nodes * nodes, sizeof(int));
    if (g->adj_matrix == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
}

void add_edge(struct Graph *g, int u, int v) {
    if (g == NULL || g->adj_matrix == NULL) return;
    if (u < 0 || u >= g->num_nodes || v < 0 || v >= g->num_nodes) {
        fprintf(stderr, "Invalid edge nodes\n");
        return;
    }
    if (g->num_edges >= MAX_EDGES) {
        fprintf(stderr, "Too many edges\n");
        return;
    }
    *(g->adj_matrix + u * g->num_nodes + v) = 1;
    *(g->adj_matrix + v * g->num_nodes + u) = 1;
    g->edges[g->num_edges][0] = u;
    g->edges[g->num_edges][1] = v;
    g->num_edges++;
}

void dfs_util(struct Graph *g, int node, int *visited) {
    if (visited == NULL) return;
    *(visited + node) = 1;
    printf("%d ", node);
    for (int i = 0; i < g->num_nodes; i++) {
        if (*(g->adj_matrix + node * g->num_nodes + i) && !*(visited + i)) {
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
    int *visited = calloc(g->num_nodes, sizeof(int));
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
    int *visited = calloc(g->num_nodes, sizeof(int));
    if (visited == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    int *queue = malloc(g->num_nodes * sizeof(int));
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
        for (int i = 0; i < g->num_nodes; i++) {
            if (*(g->adj_matrix + current * g->num_nodes + i) && !*(visited + i)) {
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

void print_graph(struct Graph *g) {
    if (g == NULL || g->adj_matrix == NULL) return;
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    for (int i = 0; i < g->num_nodes; i++) {
        printf("Node %d: ", i);
        for (int j = 0; j < g->num_nodes; j++) {
            if (*(g->adj_matrix + i * g->num_nodes + j)) {
                printf("%d ", j);
            }
        }
        printf("\n");
    }
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    init_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        free(g.adj_matrix);
        return 1;
    }
    
    if (edges < 0 || edges > MAX_ED