//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int num_nodes;
    int num_edges;
    int adj_matrix[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            *(*(g->adj_matrix + i) + j) = 0;
        }
    }
}

int add_edge(struct Graph *g, int u, int v) {
    if (u < 0 || u >= MAX_NODES || v < 0 || v >= MAX_NODES) return 0;
    if (u == v) return 0;
    if (*(*(g->adj_matrix + u) + v) == 1) return 0;
    
    *(*(g->adj_matrix + u) + v) = 1;
    *(*(g->adj_matrix + v) + u) = 1;
    g->num_edges++;
    return 1;
}

int add_node(struct Graph *g) {
    if (g->num_nodes >= MAX_NODES) return -1;
    return g->num_nodes++;
}

void dfs_util(struct Graph *g, int node, int *visited) {
    *(visited + node) = 1;
    printf("%d ", node);
    
    for (int i = 0; i < g->num_nodes; i++) {
        if (*(*(g->adj_matrix + node) + i) && !*(visited + i)) {
            dfs_util(g, i, visited);
        }
    }
}

void depth_first_search(struct Graph *g, int start) {
    if (start < 0 || start >= g->num_nodes) return;
    
    int visited[MAX_NODES];
    for (int i = 0; i < MAX_NODES; i++) {
        *(visited + i) = 0;
    }
    
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
}

void breadth_first_search(struct Graph *g, int start) {
    if (start < 0 || start >= g->num_nodes) return;
    
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
        printf("%d ", current);
        
        for (int i = 0; i < g->num_nodes; i++) {
            if (*(*(g->adj_matrix + current) + i) && !*(visited + i)) {
                *(visited + i) = 1;
                *(queue + rear) = i;
                rear++;
                if (rear >= MAX_NODES) rear = 0;
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Graph Algorithms Demo\n");
    
    for (int i = 0; i < 6; i++) {
        add_node(&graph);
    }
    
    add_edge(&graph, 0, 1);
    add_edge(&graph, 0, 2);
    add_edge(&graph, 1, 3);
    add_edge(&graph, 1, 4);
    add_edge(&graph, 2, 5);
    add_edge(&graph, 3, 4);
    
    printf("Graph has %d nodes and %d edges\n", graph.num_nodes, graph.num_edges);
    
    depth_first_search(&graph, 0);
    breadth_first_search(&graph, 0);
    
    printf("Testing invalid inputs:\n");
    add_edge(&graph, -1, 0);
    add_edge(&graph, 0, 100);
    add_edge(&graph, 0, 0);
    
    depth_first_search(&graph, -1);
    breadth_first_search(&graph, 100);
    
    return 0;
}