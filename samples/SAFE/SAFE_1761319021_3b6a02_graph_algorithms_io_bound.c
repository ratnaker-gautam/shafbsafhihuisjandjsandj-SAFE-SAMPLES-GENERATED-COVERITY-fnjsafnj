//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int nodes;
    int edges[MAX_EDGES][2];
    int edge_count;
};

void initialize_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        printf("Invalid number of nodes. Must be between 1 and %d.\n", MAX_NODES);
        exit(1);
    }
    g->nodes = nodes;
    g->edge_count = 0;
}

int add_edge(struct Graph *g, int u, int v) {
    if (g->edge_count >= MAX_EDGES) {
        printf("Maximum edge count reached.\n");
        return 0;
    }
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        printf("Invalid node indices. Must be between 0 and %d.\n", g->nodes - 1);
        return 0;
    }
    g->edges[g->edge_count][0] = u;
    g->edges[g->edge_count][1] = v;
    g->edge_count++;
    return 1;
}

void depth_first_search(struct Graph *g, int start, int visited[]) {
    if (start < 0 || start >= g->nodes) return;
    visited[start] = 1;
    printf("%d ", start);
    
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i][0] == start && !visited[g->edges[i][1]]) {
            depth_first_search(g, g->edges[i][1], visited);
        }
    }
}

void breadth_first_search(struct Graph *g, int start, int visited[]) {
    if (start < 0 || start >= g->nodes) return;
    
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i][0] == current && !visited[g->edges[i][1]]) {
                visited[g->edges[i][1]] = 1;
                queue[rear++] = g->edges[i][1];
                if (rear >= MAX_NODES) {
                    printf("Queue overflow.\n");
                    return;
                }
            }
        }
    }
}

int main() {
    struct Graph g;
    int nodes, edges;
    int visited[MAX_NODES];
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    initialize_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (edges < 0 || edges > MAX_EDGES) {
        printf("Invalid number of edges. Must be between 0 and %d.\n", MAX_EDGES);
        return 1;
    }
    
    printf("Enter edges as pairs of node indices (0 to %d):\n", nodes - 1);
    for (int i = 0; i < edges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            printf("Invalid edge input.\n");
            return 1;
        }
        if (!add_edge(&g, u, v)) {
            return 1;
        }
    }
    
    printf("\nDepth First Search traversal:\n");
    memset(visited, 0, sizeof(visited));
    for (int i = 0; i < g.nodes; i++) {
        if (!visited[i]) {
            depth_first_search(&g, i, visited);
        }
    }
    printf("\n");
    
    printf("Breadth First Search traversal:\n");
    memset(visited, 0, sizeof(visited));
    for (int i = 0; i < g.nodes; i++) {
        if (!visited[i]) {
            breadth_first_search(&g, i, visited);
        }
    }
    printf("\n");
    
    return 0;
}