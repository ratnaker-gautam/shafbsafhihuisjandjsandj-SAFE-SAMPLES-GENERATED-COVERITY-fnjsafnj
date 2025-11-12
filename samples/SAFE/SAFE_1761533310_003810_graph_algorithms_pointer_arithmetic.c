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

void initialize_graph(struct Graph *g) {
    if (g == NULL) return;
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            *(*(g->adj_matrix + i) + j) = 0;
        }
    }
}

int add_edge(struct Graph *g, int u, int v) {
    if (g == NULL) return 0;
    if (u < 0 || u >= MAX_NODES || v < 0 || v >= MAX_NODES) return 0;
    if (u == v) return 0;
    
    if (*(*(g->adj_matrix + u) + v) == 0) {
        *(*(g->adj_matrix + u) + v) = 1;
        *(*(g->adj_matrix + v) + u) = 1;
        g->num_edges++;
        return 1;
    }
    return 0;
}

void dfs_util(struct Graph *g, int v, int *visited) {
    if (g == NULL || visited == NULL) return;
    if (v < 0 || v >= MAX_NODES) return;
    
    *(visited + v) = 1;
    printf("%d ", v);
    
    for (int i = 0; i < MAX_NODES; i++) {
        if (*(*(g->adj_matrix + v) + i) && !*(visited + i)) {
            dfs_util(g, i, visited);
        }
    }
}

void depth_first_search(struct Graph *g, int start) {
    if (g == NULL) return;
    if (start < 0 || start >= MAX_NODES) return;
    
    int visited[MAX_NODES];
    for (int i = 0; i < MAX_NODES; i++) {
        *(visited + i) = 0;
    }
    
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
}

void breadth_first_search(struct Graph *g, int start) {
    if (g == NULL) return;
    if (start < 0 || start >= MAX_NODES) return;
    
    int visited[MAX_NODES];
    for (int i = 0; i < MAX_NODES; i++) {
        *(visited + i) = 0;
    }
    
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    *(visited + start) = 1;
    *(queue + rear) = start;
    rear = (rear + 1) % MAX_NODES;
    
    printf("BFS traversal: ");
    
    while (front != rear) {
        int current = *(queue + front);
        front = (front + 1) % MAX_NODES;
        printf("%d ", current);
        
        for (int i = 0; i < MAX_NODES; i++) {
            if (*(*(g->adj_matrix + current) + i) && !*(visited + i)) {
                *(visited + i) = 1;
                *(queue + rear) = i;
                rear = (rear + 1) % MAX_NODES;
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Graph Algorithms Demo\n");
    printf("Adding edges to graph...\n");
    
    add_edge(&graph, 0, 1);
    add_edge(&graph, 0, 2);
    add_edge(&graph, 1, 3);
    add_edge(&graph, 1, 4);
    add_edge(&graph, 2, 5);
    add_edge(&graph, 3, 6);
    add_edge(&graph, 4, 7);
    add_edge(&graph, 5, 8);
    
    printf("Graph created with %d edges\n", graph.num_edges);
    
    depth_first_search(&graph, 0);
    breadth_first_search(&graph, 0);
    
    printf("Adding more edges...\n");
    add_edge(&graph, 6, 9);
    add_edge(&graph, 7, 9);
    add_edge(&graph, 8, 9);
    
    printf("Graph now has %d edges\n", graph.num_edges);
    
    depth_first_search(&graph, 3);
    breadth_first_search(&graph, 3);
    
    printf("Testing invalid inputs...\n");
    add_edge(&graph, -1, 5);
    add_edge(&graph, 0, MAX_NODES);
    depth_first_search(&graph, -1);
    breadth_first_search(&graph, MAX_NODES);
    
    return 0;
}