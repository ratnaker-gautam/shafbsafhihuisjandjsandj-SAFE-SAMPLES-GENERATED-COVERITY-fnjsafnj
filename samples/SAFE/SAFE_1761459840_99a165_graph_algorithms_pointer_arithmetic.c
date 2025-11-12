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
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            *(*(g->adj_matrix + i) + j) = 0;
        }
    }
}

int add_edge(struct Graph *g, int u, int v) {
    if (u < 0 || u >= g->num_nodes || v < 0 || v >= g->num_nodes) {
        return 0;
    }
    if (*(*(g->adj_matrix + u) + v) == 0) {
        *(*(g->adj_matrix + u) + v) = 1;
        *(*(g->adj_matrix + v) + u) = 1;
        g->num_edges++;
        return 1;
    }
    return 0;
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
    if (start < 0 || start >= g->num_nodes) {
        printf("Invalid start node\n");
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
        printf("Invalid start node\n");
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
                    printf("\nQueue overflow\n");
                    return;
                }
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &graph.num_nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (graph.num_nodes < 1 || graph.num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges: ");
    if (scanf("%d", &graph.num_edges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (graph.num_edges < 0 || graph.num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (u v) for %d edges:\n", graph.num_edges);
    for (int i = 0; i < graph.num_edges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&graph, u, v)) {
            printf("Invalid edge: %d %d\n", u, v);
            return 1;
        }
    }
    
    int start_node;
    printf("Enter start node for traversals: ");
    if (scanf("%d", &start_node) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    depth_first_search(&graph, start_node);
    breadth_first_search(&graph, start_node);
    
    return 0;
}