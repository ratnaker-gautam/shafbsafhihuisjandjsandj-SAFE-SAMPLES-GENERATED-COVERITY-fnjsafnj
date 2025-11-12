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

void depth_first_search(struct Graph *g, int start, int *visited) {
    if (start < 0 || start >= g->num_nodes) {
        return;
    }
    int stack[MAX_NODES];
    int top = -1;
    *(visited + start) = 1;
    *(stack + (++top)) = start;
    while (top >= 0) {
        int current = *(stack + top--);
        printf("%d ", current);
        for (int i = 0; i < g->num_nodes; i++) {
            if (*(*(g->adj_matrix + current) + i) != 0 && *(visited + i) == 0) {
                *(visited + i) = 1;
                *(stack + (++top)) = i;
            }
        }
    }
}

void breadth_first_search(struct Graph *g, int start, int *visited) {
    if (start < 0 || start >= g->num_nodes) {
        return;
    }
    int queue[MAX_NODES];
    int front = 0;
    int rear = 0;
    *(visited + start) = 1;
    *(queue + rear) = start;
    rear = (rear + 1) % MAX_NODES;
    while (front != rear) {
        int current = *(queue + front);
        front = (front + 1) % MAX_NODES;
        printf("%d ", current);
        for (int i = 0; i < g->num_nodes; i++) {
            if (*(*(g->adj_matrix + current) + i) != 0 && *(visited + i) == 0) {
                *(visited + i) = 1;
                *(queue + rear) = i;
                rear = (rear + 1) % MAX_NODES;
            }
        }
    }
}

int main() {
    struct Graph graph;
    int nodes, edges;
    printf("Enter number of nodes (1-100): ");
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
    printf("Enter edges as u v weight (0-indexed):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&graph, u, v, w)) {
            fprintf(stderr, "Failed to add edge %d %d\n", u, v);
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