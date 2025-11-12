//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void initialize_graph(Graph *g) {
    if (g == NULL) return;
    g->node_count = 0;
    g->edge_count = 0;
}

int add_edge(Graph *g, int from, int to, int weight) {
    if (g == NULL) return 0;
    if (g->edge_count >= MAX_EDGES) return 0;
    if (from < 0 || from >= MAX_NODES) return 0;
    if (to < 0 || to >= MAX_NODES) return 0;
    if (weight < 0) return 0;
    
    g->edges[g->edge_count].from = from;
    g->edges[g->edge_count].to = to;
    g->edges[g->edge_count].weight = weight;
    g->edge_count++;
    
    if (from >= g->node_count) g->node_count = from + 1;
    if (to >= g->node_count) g->node_count = to + 1;
    
    return 1;
}

void dijkstra_algorithm(Graph *g, int start) {
    if (g == NULL) return;
    if (g->node_count == 0) return;
    if (start < 0 || start >= g->node_count) return;
    
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
                int neighbor = g->edges[i].to;
                int weight = g->edges[i].weight;
                
                if (!visited[neighbor] && dist[min_node] != INT_MAX) {
                    if (weight <= INT_MAX - dist[min_node]) {
                        int new_dist = dist[min_node] + weight;
                        if (new_dist < dist[neighbor]) {
                            dist[neighbor] = new_dist;
                        }
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
    if (g == NULL) return;
    if (g->node_count == 0) return;
    if (start < 0 || start >= g->node_count) return;
    
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
                if (!visited[neighbor] && rear < MAX_NODES) {
                    visited[neighbor] = 1;
                    queue[rear++] = neighbor;
                }
            }
        }
    }
}

void run_algorithm(Graph *g, GraphAlgorithm algorithm, int start) {
    if (g == NULL || algorithm == NULL) return;
    algorithm(g, start);
}

int main() {
    Graph graph;
    initialize_graph(&graph);
    
    add_edge(&graph, 0, 1, 4);
    add_edge(&graph, 0, 2, 1);
    add_edge(&graph, 1, 2, 2);
    add_edge(&graph, 1, 3, 5);
    add_edge(&graph, 2, 1, 2);
    add_edge(&graph, 2, 3, 8);
    add_edge(&graph, 2, 4, 10);
    add_edge(&graph, 3, 4, 2);
    add_edge(&graph, 4, 3, 2);