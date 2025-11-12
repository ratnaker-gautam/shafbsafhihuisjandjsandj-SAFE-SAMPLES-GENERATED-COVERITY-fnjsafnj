//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

typedef void (*GraphOperation)(Graph*, int, int);

void initialize_graph(Graph *g, int node_count) {
    if (g == NULL || node_count <= 0 || node_count > MAX_NODES) {
        return;
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->adjacency[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void add_edge(Graph *g, int from, int to, int weight) {
    if (g == NULL || from < 0 || from >= g->node_count || 
        to < 0 || to >= g->node_count || weight < 0) {
        return;
    }
    g->adjacency[from][to] = weight;
}

void dijkstra_operation(Graph *g, int start, int end) {
    if (g == NULL || start < 0 || start >= g->node_count || 
        end < 0 || end >= g->node_count) {
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INF;
    }
    dist[start] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_dist = INF;
        int min_index = -1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_index = v;
            }
        }
        
        if (min_index == -1 || min_index == end) {
            break;
        }
        
        visited[min_index] = 1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && g->adjacency[min_index][v] != INF && 
                dist[min_index] != INF && 
                dist[min_index] + g->adjacency[min_index][v] < dist[v]) {
                dist[v] = dist[min_index] + g->adjacency[min_index][v];
            }
        }
    }
    
    if (dist[end] != INF) {
        printf("Shortest distance from %d to %d: %d\n", start, end, dist[end]);
    } else {
        printf("No path exists from %d to %d\n", start, end);
    }
}

void bfs_operation(Graph *g, int start, int end) {
    if (g == NULL || start < 0 || start >= g->node_count || 
        end < 0 || end >= g->node_count) {
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    int distance[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        distance[i] = -1;
    }
    
    visited[start] = 1;
    distance[start] = 0;
    queue[rear++] = start;
    
    while (front < rear) {
        int current = queue[front++];
        
        if (current == end) {
            break;
        }
        
        for (int neighbor = 0; neighbor < g->node_count; neighbor++) {
            if (g->adjacency[current][neighbor] != INF && !visited[neighbor]) {
                visited[neighbor] = 1;
                distance[neighbor] = distance[current] + 1;
                queue[rear++] = neighbor;
            }
        }
    }
    
    if (distance[end] != -1) {
        printf("BFS distance from %d to %d: %d\n", start, end, distance[end]);
    } else {
        printf("No BFS path from %d to %d\n", start, end);
    }
}

void execute_graph_operation(GraphOperation op, Graph *g, int start, int end) {
    if (op != NULL && g != NULL) {
        op(g, start, end);
    }
}

int main() {
    Graph graph;
    int node_count = 6;
    
    initialize_graph(&graph, node_count);
    
    add_edge(&graph, 0, 1, 4);
    add_edge(&graph, 0, 2, 2);
    add_edge(&graph, 1, 2, 1);
    add_edge(&graph, 1, 3, 5);
    add_edge(&graph, 2, 3, 8);
    add_edge(&graph, 2, 4, 10);
    add_edge(&graph, 3, 4, 2);
    add_edge(&graph, 3, 5, 6);
    add_edge(&graph, 4, 5, 3);
    
    printf("Graph operations:\n");
    
    GraphOperation operations[] = {dijkstra_operation, bfs