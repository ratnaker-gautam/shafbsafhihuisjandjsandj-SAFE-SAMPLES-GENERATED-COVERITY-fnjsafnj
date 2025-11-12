//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
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

int add_edge(struct Graph *g, int from, int to) {
    if (from < 0 || from >= g->num_nodes || to < 0 || to >= g->num_nodes) {
        return 0;
    }
    if (*(*(g->adj_matrix + from) + to) == 0) {
        *(*(g->adj_matrix + from) + to) = 1;
        g->num_edges++;
        return 1;
    }
    return 0;
}

void depth_first_search(const struct Graph *g, int start, int *visited) {
    if (start < 0 || start >= g->num_nodes) {
        return;
    }
    *(visited + start) = 1;
    printf("%d ", start);
    for (int i = 0; i < g->num_nodes; i++) {
        if (*(*(g->adj_matrix + start) + i) && !*(visited + i)) {
            depth_first_search(g, i, visited);
        }
    }
}

void breadth_first_search(const struct Graph *g, int start, int *visited) {
    if (start < 0 || start >= g->num_nodes) {
        return;
    }
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    *(visited + start) = 1;
    *(queue + rear) = start;
    rear++;
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
}

int main() {
    struct Graph graph;
    int nodes, edges;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    initialize_graph(&graph, nodes);
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    if (edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    printf("Enter edges (from to):\n");
    for (int i = 0; i < edges; i++) {
        int from, to;
        if (scanf("%d %d", &from, &to) != 2) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&graph, from, to)) {
            fprintf(stderr, "Invalid edge: %d %d\n", from, to);
            return 1;
        }
    }
    int visited[MAX_NODES];
    for (int i = 0; i < graph.num_nodes; i++) {
        *(visited + i) = 0;
    }
    printf("DFS traversal: ");
    depth_first_search(&graph, 0, visited);
    printf("\n");
    for (int i = 0; i < graph.num_nodes; i++) {
        *(visited + i) = 0;
    }
    printf("BFS traversal: ");
    breadth_first_search(&graph, 0, visited);
    printf("\n");
    return 0;
}