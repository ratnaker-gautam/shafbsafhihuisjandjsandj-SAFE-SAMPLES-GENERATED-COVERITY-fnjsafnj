//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int num_nodes;
    int adjacency[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph *g, int n) {
    if (n <= 0 || n > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(EXIT_FAILURE);
    }
    g->num_nodes = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

void add_edge(struct Graph *g, int u, int v, int weight) {
    if (g == NULL || u < 0 || u >= g->num_nodes || v < 0 || v >= g->num_nodes) {
        fprintf(stderr, "Invalid edge parameters\n");
        exit(EXIT_FAILURE);
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        exit(EXIT_FAILURE);
    }
    g->adjacency[u][v] = weight;
    g->adjacency[v][u] = weight;
}

void dfs_util(struct Graph *g, int node, int visited[], void (*visit)(int)) {
    if (g == NULL || visited == NULL || visit == NULL) {
        return;
    }
    if (node < 0 || node >= g->num_nodes) {
        return;
    }
    visited[node] = 1;
    visit(node);
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adjacency[node][i] != 0 && !visited[i]) {
            dfs_util(g, i, visited, visit);
        }
    }
}

void dfs(struct Graph *g, int start, void (*visit)(int)) {
    if (g == NULL || visit == NULL) {
        return;
    }
    if (start < 0 || start >= g->num_nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    dfs_util(g, start, visited, visit);
}

void bfs(struct Graph *g, int start, void (*visit)(int)) {
    if (g == NULL || visit == NULL) {
        return;
    }
    if (start < 0 || start >= g->num_nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        visit(current);
        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adjacency[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                if (rear >= MAX_NODES) {
                    fprintf(stderr, "Queue overflow\n");
                    return;
                }
                queue[rear++] = i;
            }
        }
    }
}

void print_node(int node) {
    printf("%d ", node);
}

void process_graph(struct Graph *g, void (*traversal)(struct Graph*, int, void (*)(int))) {
    if (g == NULL || traversal == NULL) {
        return;
    }
    printf("Traversal: ");
    traversal(g, 0, print_node);
    printf("\n");
}

int main() {
    struct Graph g;
    int num_nodes, num_edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    initialize_graph(&g, num_nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &num_edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (num_edges < 0 || num_edges > num_nodes * (num_nodes - 1) / 2) {
        fprintf(stderr, "Invalid number of edges\n");
        return EXIT_FAILURE;
    }
    
    for (int i = 0; i < num_edges; i++) {
        int u, v, weight;
        printf("Enter edge %d (u v weight): ", i + 1);
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return EXIT_FAILURE;
        }
        add_edge(&g, u, v, weight);
    }
    
    printf("\nDFS traversal: ");
    process_graph(&g, dfs);
    
    printf("BFS traversal: ");
    process_graph(&g, bfs);
    
    return EXIT_SUCCESS;
}