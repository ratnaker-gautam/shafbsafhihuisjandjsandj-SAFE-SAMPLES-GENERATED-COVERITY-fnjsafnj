//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

typedef void (*GraphOperation)(Graph*, int, int);

void initialize_graph(Graph *g, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(EXIT_FAILURE);
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid node index\n");
        exit(EXIT_FAILURE);
    }
    if (weight < 0 || weight > 1000) {
        fprintf(stderr, "Invalid edge weight\n");
        exit(EXIT_FAILURE);
    }
    g->adjacency[from][to] = weight;
    g->adjacency[to][from] = weight;
}

void dfs_traversal(Graph *g, int start, int target) {
    if (start < 0 || start >= g->node_count || target < 0 || target >= g->node_count) {
        fprintf(stderr, "Invalid start or target node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES];
    int top = 0;
    
    stack[top++] = start;
    visited[start] = 1;
    
    printf("DFS traversal from %d: ", start);
    
    while (top > 0) {
        int current = stack[--top];
        printf("%d ", current);
        
        if (current == target) {
            printf("(target found)");
            break;
        }
        
        for (int i = g->node_count - 1; i >= 0; i--) {
            if (g->adjacency[current][i] > 0 && !visited[i]) {
                if (top >= MAX_NODES) {
                    fprintf(stderr, "Stack overflow\n");
                    return;
                }
                stack[top++] = i;
                visited[i] = 1;
            }
        }
    }
    printf("\n");
}

void bfs_traversal(Graph *g, int start, int target) {
    if (start < 0 || start >= g->node_count || target < 0 || target >= g->node_count) {
        fprintf(stderr, "Invalid start or target node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    queue[rear++] = start;
    visited[start] = 1;
    
    printf("BFS traversal from %d: ", start);
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        if (current == target) {
            printf("(target found)");
            break;
        }
        
        for (int i = 0; i < g->node_count; i++) {
            if (g->adjacency[current][i] > 0 && !visited[i]) {
                if (rear >= MAX_NODES) {
                    fprintf(stderr, "Queue overflow\n");
                    return;
                }
                queue[rear++] = i;
                visited[i] = 1;
            }
        }
    }
    printf("\n");
}

void dijkstra_shortest_path(Graph *g, int start, int target) {
    if (start < 0 || start >= g->node_count || target < 0 || target >= g->node_count) {
        fprintf(stderr, "Invalid start or target node\n");
        return;
    }
    
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
        
        for (int i = 0; i < g->node_count; i++) {
            if (!visited[i] && g->adjacency[min_node][i] > 0) {
                int new_dist;
                if (dist[min_node] > INT_MAX - g->adjacency[min_node][i]) {
                    continue;
                }
                new_dist = dist[min_node] + g->adjacency[min_node][i];
                if (new_dist < dist[i]) {
                    dist[i] = new_dist;
                }
            }
        }
    }
    
    printf("Short