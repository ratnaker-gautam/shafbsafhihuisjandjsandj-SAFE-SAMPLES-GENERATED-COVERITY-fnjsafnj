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

int add_edge(struct Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->num_nodes || v < 0 || v >= g->num_nodes) {
        return 0;
    }
    if (u == v) {
        return 0;
    }
    if (*(*(g->adj_matrix + u) + v) == 0) {
        *(*(g->adj_matrix + u) + v) = weight;
        *(*(g->adj_matrix + v) + u) = weight;
        g->num_edges++;
        return 1;
    }
    return 0;
}

void dfs_util(struct Graph *g, int node, int *visited) {
    *(visited + node) = 1;
    printf("%d ", node);
    for (int i = 0; i < g->num_nodes; i++) {
        if (*(*(g->adj_matrix + node) + i) != 0 && *(visited + i) == 0) {
            dfs_util(g, i, visited);
        }
    }
}

void depth_first_search(struct Graph *g, int start) {
    if (start < 0 || start >= g->num_nodes) {
        fprintf(stderr, "Invalid start node\n");
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
    if (start < 0 || start >= g->num_nodes) {
        fprintf(stderr, "Invalid start node\n");
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
            if (*(*(g->adj_matrix + current) + i) != 0 && *(visited + i) == 0) {
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
    struct Graph g;
    int num_nodes = 6;
    initialize_graph(&g, num_nodes);
    add_edge(&g, 0, 1, 1);
    add_edge(&g, 0, 2, 1);
    add_edge(&g, 1, 3, 1);
    add_edge(&g, 1, 4, 1);
    add_edge(&g, 2, 4, 1);
    add_edge(&g, 3, 5, 1);
    add_edge(&g, 4, 5, 1);
    depth_first_search(&g, 0);
    breadth_first_search(&g, 0);
    return 0;
}