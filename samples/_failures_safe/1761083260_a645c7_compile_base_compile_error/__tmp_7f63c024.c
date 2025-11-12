//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
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

typedef int (*GraphOperation)(Graph*, int, int);

void initialize_graph(Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        nodes = MAX_NODES;
    }
    g->node_count = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adjacency[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void add_edge(Graph *g, int u, int v, int weight) {
    if (g == NULL || u < 0 || u >= g->node_count || v < 0 || v >= g->node_count) {
        return;
    }
    if (weight < 0 || weight == INF) {
        return;
    }
    g->adjacency[u][v] = weight;
    g->adjacency[v][u] = weight;
}

int dijkstra_shortest_path(Graph *g, int start, int end) {
    if (g == NULL || start < 0 || start >= g->node_count || end < 0 || end >= g->node_count) {
        return INF;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[start] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_dist = INF;
        int min_node = -1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_node = v;
            }
        }
        
        if (min_node == -1 || min_node == end) {
            break;
        }
        
        visited[min_node] = 1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && g->adjacency[min_node][v] != INF && dist[min_node] != INF) {
                int new_dist = dist[min_node] + g->adjacency[min_node][v];
                if (new_dist > 0 && new_dist < dist[v]) {
                    dist[v] = new_dist;
                }
            }
        }
    }
    
    return dist[end];
}

int bfs_shortest_path(Graph *g, int start, int end) {
    if (g == NULL || start < 0 || start >= g->node_count || end < 0 || end >= g->node_count) {
        return INF;
    }
    
    int visited[MAX_NODES];
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    int dist[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        visited[i] = 0;
        dist[i] = INF;
    }
    
    visited[start] = 1;
    dist[start] = 0;
    queue[rear++] = start;
    
    while (front < rear) {
        int current = queue[front++];
        
        if (current == end) {
            break;
        }
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && g->adjacency[current][v] == 1) {
                visited[v] = 1;
                dist[v] = dist[current] + 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = v;
                }
            }
        }
    }
    
    return dist[end];
}

int execute_graph_operation(GraphOperation op, Graph *g, int start, int end) {
    if (op == NULL || g == NULL) {
        return INF;
    }
    return op(g, start, end);
}

void print_graph(Graph *g) {
    if (g == NULL) {
        return;
    }
    printf("Graph adjacency matrix (%d nodes):\n", g->node_count);
    for (int i = 0; i < g->node_count; i++) {
        for (int j = 0; j < g->node_count; j++) {
            if (g->adjacency[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", g->adjacency[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    Graph g;
    int nodes = 6;
    
    initialize_graph(&g, nodes);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 2);
    add_edge(&g, 1, 2, 1);
    add_edge(&g, 1, 3, 5);
    add_edge(&g, 2, 3, 8);
    add_edge(&g, 2, 4, 10);
    add_edge(&g, 3,