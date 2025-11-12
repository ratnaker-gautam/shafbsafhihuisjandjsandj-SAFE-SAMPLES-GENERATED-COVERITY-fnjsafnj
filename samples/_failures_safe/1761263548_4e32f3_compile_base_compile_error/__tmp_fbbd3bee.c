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

void add_edge(Graph *g, int from, int to) {
    if (g == NULL || from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid node indices\n");
        return;
    }
    g->adjacency[from][to] = 1;
    g->adjacency[to][from] = 1;
}

void dfs_visit(Graph *g, int node, int visited[]) {
    if (g == NULL || node < 0 || node >= g->node_count) return;
    
    visited[node] = 1;
    printf("%d ", node);
    
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] && !visited[i]) {
            dfs_visit(g, i, visited);
        }
    }
}

void depth_first_search(Graph *g, int start, int target) {
    if (g == NULL || start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    printf("DFS from %d: ", start);
    dfs_visit(g, start, visited);
    printf("\n");
}

void breadth_first_search(Graph *g, int start, int target) {
    if (g == NULL || start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS from %d: ", start);
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->node_count; i++) {
            if (g->adjacency[current][i] && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
    printf("\n");
}

void find_shortest_path(Graph *g, int start, int target) {
    if (g == NULL || start < 0 || start >= g->node_count || target < 0 || target >= g->node_count) {
        fprintf(stderr, "Invalid node indices\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int distance[MAX_NODES];
    int previous[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        distance[i] = INT_MAX;
        previous[i] = -1;
    }
    
    distance[start] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_distance = INT_MAX;
        int min_node = -1;
        
        for (int i = 0; i < g->node_count; i++) {
            if (!visited[i] && distance[i] < min_distance) {
                min_distance = distance[i];
                min_node = i;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int i = 0; i < g->node_count; i++) {
            if (g->adjacency[min_node][i] && !visited[i]) {
                int new_distance = distance[min_node] + 1;
                if (new_distance < distance[i]) {
                    distance[i] = new_distance;
                    previous[i] = min_node;
                }
            }
        }
    }
    
    if (distance[target] == INT_MAX) {
        printf("No path from %d to %d\n", start, target);
    } else {
        printf("Shortest path from %d to %d: ", start, target);
        int path[MAX_NODES];
        int path_length = 0;
        int current = target;
        
        while (current != -1) {
            if (path_length >= MAX_NODES) break;
            path[path_length++] = current;
            current = previous[current];
        }
        
        for (int i = path_length - 1; i >= 0; i--) {
            printf("%d", path[i]);
            if (i > 0) printf(" -> ");
        }
        printf(" (distance: %d)\n", distance[target]);
    }
}