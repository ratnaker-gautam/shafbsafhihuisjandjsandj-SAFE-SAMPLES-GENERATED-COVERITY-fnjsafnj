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

void initialize_graph(Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    g->node_count = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adjacency[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid node index\n");
        return;
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        return;
    }
    g->adjacency[from][to] = weight;
}

void print_graph(Graph *g, int start, int end) {
    if (start < 0 || start >= g->node_count || end < 0 || end >= g->node_count) {
        fprintf(stderr, "Invalid node range\n");
        return;
    }
    printf("Adjacency matrix:\n");
    for (int i = start; i <= end && i < g->node_count; i++) {
        for (int j = start; j <= end && j < g->node_count; j++) {
            if (g->adjacency[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", g->adjacency[i][j]);
            }
        }
        printf("\n");
    }
}

void dijkstra(Graph *g, int source, int target) {
    if (source < 0 || source >= g->node_count || target < 0 || target >= g->node_count) {
        fprintf(stderr, "Invalid source or target\n");
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[source] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_dist = INF;
        int min_index = -1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_index = v;
            }
        }
        
        if (min_index == -1 || min_dist == INF) break;
        visited[min_index] = 1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && g->adjacency[min_index][v] != INF && 
                dist[min_index] != INF) {
                long new_dist = (long)dist[min_index] + g->adjacency[min_index][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
    
    printf("Shortest distance from %d to %d: ", source, target);
    if (dist[target] == INF) {
        printf("No path\n");
    } else {
        printf("%d\n", dist[target]);
    }
}

void bfs_traversal(Graph *g, int start, int unused) {
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS traversal from node %d: ", start);
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->node_count; i++) {
            if (g->adjacency[current][i] != INF && g->adjacency[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
    printf("\n");
}

void execute_operation(Graph *g, GraphOperation op, int param1, int param2) {
    if (g == NULL || op == NULL) {
        fprintf(stderr, "Invalid graph or operation\n");
        return;
    }
    op(g, param1, param2);
}

int main() {
    Graph g;
    int nodes = 6;
    
    initialize_graph(&g, nodes);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 2);
    add_edge(&g,