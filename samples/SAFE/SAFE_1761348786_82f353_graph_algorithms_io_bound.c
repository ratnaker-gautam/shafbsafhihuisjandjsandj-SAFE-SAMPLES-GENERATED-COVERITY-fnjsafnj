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
    if (nodes < 1 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->nodes = nodes;
    g->edge_count = 0;
}

void add_edge(struct Graph *g, int u, int v) {
    if (g->edge_count >= MAX_EDGES) {
        fprintf(stderr, "Too many edges\n");
        exit(1);
    }
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    g->edges[g->edge_count][0] = u;
    g->edges[g->edge_count][1] = v;
    g->edge_count++;
}

void bfs(struct Graph *g, int start) {
    if (start < 0 || start >= g->nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES];
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    for (int i = 0; i < g->nodes; i++) {
        visited[i] = 0;
    }
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS traversal from node %d: ", start);
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i][0] == current) {
                int neighbor = g->edges[i][1];
                if (!visited[neighbor]) {
                    visited[neighbor] = 1;
                    if (rear < MAX_NODES) {
                        queue[rear++] = neighbor;
                    } else {
                        fprintf(stderr, "Queue overflow\n");
                        return;
                    }
                }
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initialize_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as pairs of node indices (0 to %d):\n", nodes - 1);
    for (int i = 0; i < edges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&g, u, v);
    }
    
    int start_node;
    printf("Enter starting node for BFS: ");
    if (scanf("%d", &start_node) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    bfs(&g, start_node);
    
    return 0;
}