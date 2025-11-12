//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 500

struct Graph {
    int num_nodes;
    int num_edges;
    int adj_matrix[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = 0;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            *(*(g->adj_matrix + i) + j) = 0;
        }
    }
}

void add_edge(struct Graph *g, int u, int v) {
    if (u < 0 || u >= g->num_nodes || v < 0 || v >= g->num_nodes) {
        fprintf(stderr, "Invalid edge nodes\n");
        return;
    }
    if (*(*(g->adj_matrix + u) + v) == 0) {
        *(*(g->adj_matrix + u) + v) = 1;
        *(*(g->adj_matrix + v) + u) = 1;
        g->num_edges++;
    }
}

void dfs_util(struct Graph *g, int node, int *visited) {
    if (node < 0 || node >= g->num_nodes || visited == NULL) return;
    *(visited + node) = 1;
    printf("%d ", node);
    for (int i = 0; i < g->num_nodes; i++) {
        if (*(*(g->adj_matrix + node) + i) && !*(visited + i)) {
            dfs_util(g, i, visited);
        }
    }
}

void depth_first_search(struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->num_nodes) {
        fprintf(stderr, "Invalid DFS parameters\n");
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        *(visited + i) = 0;
    }
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
}

void breadth_first_search(struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->num_nodes) {
        fprintf(stderr, "Invalid BFS parameters\n");
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        *(visited + i) = 0;
    }
    int queue[MAX_NODES];
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
            if (*(*(g->adj_matrix + current) + i) && !*(visited + i)) {
                *(visited + i) = 1;
                *(queue + rear) = i;
                rear++;
                if (rear >= MAX_NODES) {
                    fprintf(stderr, "Queue overflow\n");
                    return;
                }
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph graph;
    int num_nodes, num_edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &num_nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initialize_graph(&graph, num_nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &num_edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (num_edges < 0 || num_edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (u v pairs):\n");
    for (int i = 0; i < num_edges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&graph, u, v);
    }
    
    int start_node;
    printf("Enter starting node for traversals: ");
    if (scanf("%d", &start_node) != 1) {
        fprintf(stderr, "Invalid start node\n");
        return 1;
    }
    
    if (start_node < 0 || start_node >= graph.num_nodes) {
        fprintf(stderr, "Invalid start node\n");
        return 1;
    }
    
    depth_first_search(&graph, start_node);
    breadth_first_search(&graph, start_node);
    
    return 0;
}