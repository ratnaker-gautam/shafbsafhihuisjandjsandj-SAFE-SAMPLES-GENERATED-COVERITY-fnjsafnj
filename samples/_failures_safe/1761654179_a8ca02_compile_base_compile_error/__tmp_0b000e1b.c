//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 500

struct Graph {
    int node_count;
    int edge_count;
    int adjacency[MAX_NODES][MAX_NODES];
    int visited[MAX_NODES];
    int distance[MAX_NODES];
};

void initialize_graph(struct Graph *g) {
    if (g == NULL) return;
    g->node_count = 0;
    g->edge_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        *(g->visited + i) = 0;
        *(g->distance + i) = INT_MAX;
        for (int j = 0; j < MAX_NODES; j++) {
            *(*(g->adjacency + i) + j) = 0;
        }
    }
}

int add_edge(struct Graph *g, int from, int to, int weight) {
    if (g == NULL) return 0;
    if (from < 0 || from >= MAX_NODES) return 0;
    if (to < 0 || to >= MAX_NODES) return 0;
    if (weight < 0) return 0;
    
    if (*(*(g->adjacency + from) + to) == 0) {
        g->edge_count++;
        if (g->edge_count > MAX_EDGES) {
            g->edge_count--;
            return 0;
        }
    }
    *(*(g->adjacency + from) + to) = weight;
    
    if (from + 1 > g->node_count) g->node_count = from + 1;
    if (to + 1 > g->node_count) g->node_count = to + 1;
    
    return 1;
}

void dfs(struct Graph *g, int node) {
    if (g == NULL) return;
    if (node < 0 || node >= MAX_NODES) return;
    
    *(g->visited + node) = 1;
    printf("Visited node: %d\n", node);
    
    for (int i = 0; i < g->node_count; i++) {
        if (*(*(g->adjacency + node) + i) > 0 && *(g->visited + i) == 0) {
            dfs(g, i);
        }
    }
}

void bfs(struct Graph *g, int start) {
    if (g == NULL) return;
    if (start < 0 || start >= MAX_NODES) return;
    
    for (int i = 0; i < MAX_NODES; i++) {
        *(g->visited + i) = 0;
        *(g->distance + i) = INT_MAX;
    }
    
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    int queue_size = 0;
    
    *(g->visited + start) = 1;
    *(g->distance + start) = 0;
    *(queue + rear) = start;
    rear = (rear + 1) % MAX_NODES;
    queue_size++;
    
    while (queue_size > 0) {
        int current = *(queue + front);
        front = (front + 1) % MAX_NODES;
        queue_size--;
        
        printf("Node: %d, Distance: %d\n", current, *(g->distance + current));
        
        for (int i = 0; i < g->node_count; i++) {
            if (*(*(g->adjacency + current) + i) > 0 && *(g->visited + i) == 0) {
                *(g->visited + i) = 1;
                *(g->distance + i) = *(g->distance + current) + 1;
                if (queue_size < MAX_NODES) {
                    *(queue + rear) = i;
                    rear = (rear + 1) % MAX_NODES;
                    queue_size++;
                }
            }
        }
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Graph Algorithms Demo\n");
    printf("Adding edges to graph...\n");
    
    if (!add_edge(&graph, 0, 1, 1)) return 1;
    if (!add_edge(&graph, 0, 2, 1)) return 1;
    if (!add_edge(&graph, 1, 3, 1)) return 1;
    if (!add_edge(&graph, 2, 3, 1)) return 1;
    if (!add_edge(&graph, 3, 4, 1)) return 1;
    if (!add_edge(&graph, 4, 5, 1)) return 1;
    
    printf("\nDepth-First Search starting from node 0:\n");
    for (int i = 0; i < MAX_NODES; i++) {
        *(graph.visited + i) = 0;
    }
    dfs(&graph, 0);
    
    printf("\nBreadth-First Search starting from node 0:\n");
    bfs(&graph, 0);
    
    printf("\nAdding more edges for demonstration...\n");
    if (!add_edge(&graph, 5, 0, 1)) return 1;
    if (!add_edge(&graph, 2, 5, 1)) return 1;