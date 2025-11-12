//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int num_nodes;
    int adj_matrix[MAX_NODES][MAX_NODES];
};

void init_graph(struct Graph *g, int n) {
    if (n <= 0 || n > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->num_nodes = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g->adj_matrix[i][j] = 0;
        }
    }
}

void add_edge(struct Graph *g, int u, int v, int weight) {
    if (g == NULL || u < 0 || u >= g->num_nodes || v < 0 || v >= g->num_nodes) {
        fprintf(stderr, "Invalid edge parameters\n");
        exit(1);
    }
    g->adj_matrix[u][v] = weight;
    g->adj_matrix[v][u] = weight;
}

void dfs_util(struct Graph *g, int node, int visited[], void (*visit)(int)) {
    if (g == NULL || visited == NULL || visit == NULL) return;
    if (node < 0 || node >= g->num_nodes) return;
    
    visited[node] = 1;
    visit(node);
    
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adj_matrix[node][i] != 0 && !visited[i]) {
            dfs_util(g, i, visited, visit);
        }
    }
}

void dfs(struct Graph *g, int start, void (*visit)(int)) {
    if (g == NULL || visit == NULL) return;
    if (start < 0 || start >= g->num_nodes) return;
    
    int visited[MAX_NODES] = {0};
    dfs_util(g, start, visited, visit);
}

void bfs(struct Graph *g, int start, void (*visit)(int)) {
    if (g == NULL || visit == NULL) return;
    if (start < 0 || start >= g->num_nodes) return;
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    while (front < rear) {
        int current = queue[front++];
        visit(current);
        
        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adj_matrix[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
}

void dijkstra(struct Graph *g, int start, void (*process)(int, int)) {
    if (g == NULL || process == NULL) return;
    if (start < 0 || start >= g->num_nodes) return;
    
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    
    for (int count = 0; count < g->num_nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int i = 0; i < g->num_nodes; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                min_node = i;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int i = 0; i < g->num_nodes; i++) {
            if (!visited[i] && g->adj_matrix[min_node][i] != 0 && dist[min_node] != INT_MAX) {
                int new_dist = dist[min_node] + g->adj_matrix[min_node][i];
                if (new_dist > 0 && new_dist < dist[i]) {
                    dist[i] = new_dist;
                }
            }
        }
    }
    
    for (int i = 0; i < g->num_nodes; i++) {
        process(i, dist[i]);
    }
}

void print_node(int node) {
    printf("Visited node: %d\n", node);
}

void print_distance(int node, int distance) {
    if (distance == INT_MAX) {
        printf("Node %d: unreachable\n", node);
    } else {
        printf("Node %d: distance %d\n", node, distance);
    }
}

void run_algorithm(struct Graph *g, int start, void (*algorithm)(struct Graph*, int, void (*)(int))) {
    if (g == NULL || algorithm == NULL) return;
    printf("Running algorithm from node %d:\n", start);
    algorithm(g, start, print_node);
    printf("\n");
}

void run_dijkstra(struct Graph *g, int start) {
    if (g == NULL) return;
    printf("Running Dijkstra from node %d:\n", start);
    dijkstra(g, start, print_distance);
    printf("\n");
}

int main() {
    struct Graph g;