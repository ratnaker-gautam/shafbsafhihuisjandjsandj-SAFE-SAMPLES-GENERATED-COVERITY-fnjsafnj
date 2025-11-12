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
        exit(1);
    }
    if (g->node_count <= 0 || g->node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    if (g->edge_count < 0 || g->edge_count > MAX_EDGES) {
        fprintf(stderr, "Invalid edge count\n");
        exit(1);
    }
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from < 0 || g->edges[i].from >= g->node_count ||
            g->edges[i].to < 0 || g->edges[i].to >= g->node_count) {
            fprintf(stderr, "Invalid edge node indices\n");
            exit(1);
        }
    }
}

void dijkstra_algorithm(Graph *g, int start) {
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[start] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_node = v;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == min_node) {
                int to = g->edges[i].to;
                int weight = g->edges[i].weight;
                if (weight < 0) {
                    fprintf(stderr, "Negative weights not supported\n");
                    return;
                }
                if (!visited[to] && dist[min_node] != INT_MAX) {
                    int new_dist = dist[min_node] + weight;
                    if (new_dist < dist[to]) {
                        dist[to] = new_dist;
                    }
                }
            }
        }
    }
    
    printf("Dijkstra results from node %d:\n", start);
    for (int i = 0; i < g->node_count; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: distance %d\n", i, dist[i]);
        }
    }
}

void bfs_algorithm(Graph *g, int start) {
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS traversal from node %d:\n", start);
    
    while (front < rear) {
        int current = queue[front++];
        printf("Visited node %d\n", current);
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == current) {
                int neighbor = g->edges[i].to;
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
}

void run_algorithm(GraphAlgorithm algo, Graph *g, int start) {
    if (algo == NULL) {
        fprintf(stderr, "Algorithm function is NULL\n");
        return;
    }
    validate_graph(g);
    algo(g, start);
}

Graph create_sample_graph() {
    Graph g;
    g.node_count = 6;
    g.edge_count = 9;
    
    Edge edges[] = {
        {0, 1, 4}, {0, 2, 2}, {1, 2, 1},
        {1, 3, 5}, {2, 3, 8}, {2, 4, 10},
        {3, 4, 2}, {3, 5, 6}, {4, 5, 3}
    };
    
    for (int i = 0; i < g.edge_count; i++) {
        g.edges[i] =