//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

typedef struct {
    int from;
    int to;
    int weight;
} Edge;

typedef struct {
    int node_count;
    int edge_count;
    Edge edges[MAX_EDGES];
} Graph;

typedef void (*GraphAlgorithm)(Graph*, int);

void validate_graph(Graph *g) {
    if (g == NULL) {
        fprintf(stderr, "Graph is NULL\n");
        exit(EXIT_FAILURE);
    }
    if (g->node_count <= 0 || g->node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count: %d\n", g->node_count);
        exit(EXIT_FAILURE);
    }
    if (g->edge_count < 0 || g->edge_count > MAX_EDGES) {
        fprintf(stderr, "Invalid edge count: %d\n", g->edge_count);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from < 0 || g->edges[i].from >= g->node_count ||
            g->edges[i].to < 0 || g->edges[i].to >= g->node_count) {
            fprintf(stderr, "Invalid edge: %d -> %d\n", g->edges[i].from, g->edges[i].to);
            exit(EXIT_FAILURE);
        }
    }
}

void validate_start_node(Graph *g, int start) {
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node: %d\n", start);
        exit(EXIT_FAILURE);
    }
}

void bfs_traversal(Graph *g, int start) {
    validate_graph(g);
    validate_start_node(g, start);
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS traversal from node %d: ", start);
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == current && !visited[g->edges[i].to]) {
                visited[g->edges[i].to] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = g->edges[i].to;
                } else {
                    fprintf(stderr, "Queue overflow\n");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    printf("\n");
}

void dfs_util(Graph *g, int node, int visited[]) {
    printf("%d ", node);
    visited[node] = 1;
    
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from == node && !visited[g->edges[i].to]) {
            dfs_util(g, g->edges[i].to, visited);
        }
    }
}

void dfs_traversal(Graph *g, int start) {
    validate_graph(g);
    validate_start_node(g, start);
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from node %d: ", start);
    dfs_util(g, start, visited);
    printf("\n");
}

void dijkstra_shortest_path(Graph *g, int start) {
    validate_graph(g);
    validate_start_node(g, start);
    
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int i = 0; i < g->node_count; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                min_node = i;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == min_node) {
                int to = g->edges[i].to;
                int weight = g->edges[i].weight;
                if (weight < 0) {
                    fprintf(stderr, "Negative weight detected\n");
                    exit(EXIT_FAILURE);
                }
                if (!visited[to] && dist[min_node] != INT_MAX) {
                    long new_dist = (long)dist[min_node] + weight;
                    if (new_dist < INT_MAX && new_dist < dist[to]) {
                        dist[to] = new_dist;
                    }
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < g->node_count; i++) {
        if (dist[i] == INT_MAX) {
            printf