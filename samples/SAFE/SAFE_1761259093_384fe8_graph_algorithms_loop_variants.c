//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int edges[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

void initialize_graph(Graph *g, int n) {
    if (n <= 0 || n > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    g->node_count = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g->edges[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->node_count || v < 0 || v >= g->node_count) {
        fprintf(stderr, "Invalid edge\n");
        exit(1);
    }
    g->edges[u][v] = weight;
    g->edges[v][u] = weight;
}

void dijkstra_shortest_path(Graph *g, int source) {
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[source] = 0;
    
    int count = 0;
    while (count < g->node_count) {
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
        count++;
        
        for (int i = 0; i < g->node_count; i++) {
            if (!visited[i] && g->edges[min_node][i] != 0) {
                int new_dist;
                if (dist[min_node] < INT_MAX - g->edges[min_node][i]) {
                    new_dist = dist[min_node] + g->edges[min_node][i];
                } else {
                    new_dist = INT_MAX;
                }
                if (new_dist < dist[i]) {
                    dist[i] = new_dist;
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < g->node_count; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

void depth_first_search(Graph *g, int start) {
    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES];
    int top = -1;
    
    stack[++top] = start;
    visited[start] = 1;
    
    printf("DFS traversal: ");
    while (top >= 0) {
        int current = stack[top--];
        printf("%d ", current);
        
        for (int i = g->node_count - 1; i >= 0; i--) {
            if (g->edges[current][i] != 0 && !visited[i]) {
                stack[++top] = i;
                visited[i] = 1;
            }
        }
    }
    printf("\n");
}

void breadth_first_search(Graph *g, int start) {
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    queue[rear++] = start;
    visited[start] = 1;
    
    printf("BFS traversal: ");
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->node_count; i++) {
            if (g->edges[current][i] != 0 && !visited[i]) {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }
    }
    printf("\n");
}

int main() {
    Graph g;
    int node_count = 6;
    
    initialize_graph(&g, node_count);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 2);
    add_edge(&g, 1, 2, 1);
    add_edge(&g, 1, 3, 5);
    add_edge(&g, 2, 3, 8);
    add_edge(&g, 2, 4, 10);
    add_edge(&g, 3, 4, 2);
    add_edge(&g, 3, 5, 6);
    add_edge(&g, 4, 5, 3);
    
    printf("Graph algorithms demonstration:\n\n");
    
    dijkstra_shortest_path(&g, 0);
    printf("\n");
    
    depth_first_search(&g, 0);
    printf("\n");
    
    breadth_first_search(&g, 0);
    
    return 0;
}