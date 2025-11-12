//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int nodes;
    int adj[MAX_NODES][MAX_NODES];
};

struct Graph* create_graph(int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        return NULL;
    }
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            graph->adj[i][j] = 0;
        }
    }
    return graph;
}

int add_edge(struct Graph* graph, int src, int dest) {
    if (graph == NULL) {
        return 0;
    }
    if (src < 0 || src >= graph->nodes || dest < 0 || dest >= graph->nodes) {
        return 0;
    }
    graph->adj[src][dest] = 1;
    graph->adj[dest][src] = 1;
    return 1;
}

void dfs_util(struct Graph* graph, int node, int visited[]) {
    if (graph == NULL || visited == NULL) {
        return;
    }
    if (node < 0 || node >= graph->nodes) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < graph->nodes; i++) {
        if (graph->adj[node][i] && !visited[i]) {
            dfs_util(graph, i, visited);
        }
    }
}

void depth_first_search(struct Graph* graph, int start) {
    if (graph == NULL) {
        return;
    }
    if (start < 0 || start >= graph->nodes) {
        printf("Invalid start node\n");
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < graph->nodes; i++) {
        visited[i] = 0;
    }
    printf("DFS traversal: ");
    dfs_util(graph, start, visited);
    printf("\n");
}

void breadth_first_search(struct Graph* graph, int start) {
    if (graph == NULL) {
        return;
    }
    if (start < 0 || start >= graph->nodes) {
        printf("Invalid start node\n");
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < graph->nodes; i++) {
        visited[i] = 0;
    }
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    printf("BFS traversal: ");
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        for (int i = 0; i < graph->nodes; i++) {
            if (graph->adj[current][i] && !visited[i]) {
                visited[i] = 1;
                queue[rear++] = i;
                if (rear >= MAX_NODES) {
                    printf("\nQueue overflow\n");
                    return;
                }
            }
        }
    }
    printf("\n");
}

int main() {
    int nodes, edges;
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    struct Graph* graph = create_graph(nodes);
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        printf("Invalid input\n");
        free(graph);
        return 1;
    }
    if (edges < 0 || edges > nodes * (nodes - 1) / 2) {
        printf("Invalid number of edges\n");
        free(graph);
        return 1;
    }
    printf("Enter edges (source destination):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest;
        if (scanf("%d %d", &src, &dest) != 2) {
            printf("Invalid edge input\n");
            free(graph);
            return 1;
        }
        if (!add_edge(graph, src, dest)) {
            printf("Invalid edge: %d %d\n", src, dest);
            free(graph);
            return 1;
        }
    }
    int start_node;
    printf("Enter start node for traversals: ");
    if (scanf("%d", &start_node) != 1) {
        printf("Invalid input\n");
        free(graph);
        return 1;
    }
    depth_first_search(graph, start_node);
    breadth_first_search(graph, start_node);
    free(graph);
    return 0;
}