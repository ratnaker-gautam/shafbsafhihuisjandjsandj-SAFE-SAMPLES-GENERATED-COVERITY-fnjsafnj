//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 500

struct Graph {
    int nodes[MAX_NODES];
    int edges[MAX_NODES][MAX_NODES];
    int node_count;
    int edge_count;
};

void init_graph(struct Graph *g) {
    g->node_count = 0;
    g->edge_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        *(g->nodes + i) = -1;
        for (int j = 0; j < MAX_NODES; j++) {
            *(*(g->edges + i) + j) = 0;
        }
    }
}

int add_node(struct Graph *g, int value) {
    if (g->node_count >= MAX_NODES) return -1;
    if (value < 0 || value > 1000) return -1;
    *(g->nodes + g->node_count) = value;
    g->node_count++;
    return g->node_count - 1;
}

int add_edge(struct Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count) return -1;
    if (to < 0 || to >= g->node_count) return -1;
    if (weight < 0 || weight > 100) return -1;
    if (*(*(g->edges + from) + to) != 0) return -1;
    if (g->edge_count >= MAX_EDGES) return -1;
    
    *(*(g->edges + from) + to) = weight;
    *(*(g->edges + to) + from) = weight;
    g->edge_count++;
    return 0;
}

void dfs_util(struct Graph *g, int node, int *visited) {
    if (node < 0 || node >= g->node_count) return;
    if (*(visited + node)) return;
    
    *(visited + node) = 1;
    printf("%d ", *(g->nodes + node));
    
    for (int i = 0; i < g->node_count; i++) {
        if (*(*(g->edges + node) + i) != 0 && !*(visited + i)) {
            dfs_util(g, i, visited);
        }
    }
}

void depth_first_search(struct Graph *g, int start) {
    if (start < 0 || start >= g->node_count) return;
    
    int visited[MAX_NODES];
    for (int i = 0; i < MAX_NODES; i++) {
        *(visited + i) = 0;
    }
    
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
}

void breadth_first_search(struct Graph *g, int start) {
    if (start < 0 || start >= g->node_count) return;
    
    int visited[MAX_NODES];
    for (int i = 0; i < MAX_NODES; i++) {
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
        printf("%d ", *(g->nodes + current));
        
        for (int i = 0; i < g->node_count; i++) {
            if (*(*(g->edges + current) + i) != 0 && !*(visited + i)) {
                *(visited + i) = 1;
                *(queue + rear) = i;
                rear++;
                if (rear >= MAX_NODES) rear = MAX_NODES - 1;
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph graph;
    init_graph(&graph);
    
    int node_count;
    printf("Enter number of nodes (1-20): ");
    if (scanf("%d", &node_count) != 1 || node_count < 1 || node_count > 20) {
        printf("Invalid node count\n");
        return 1;
    }
    
    printf("Enter node values:\n");
    for (int i = 0; i < node_count; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            printf("Invalid input\n");
            return 1;
        }
        if (add_node(&graph, value) == -1) {
            printf("Failed to add node\n");
            return 1;
        }
    }
    
    int edge_count;
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1 || edge_count < 0 || edge_count > 50) {
        printf("Invalid edge count\n");
        return 1;
    }
    
    printf("Enter edges (from to weight):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (add_edge(&graph, from, to, weight) == -1) {