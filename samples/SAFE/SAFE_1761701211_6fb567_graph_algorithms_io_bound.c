//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 500

struct Graph {
    int num_nodes;
    int num_edges;
    int adjacency[MAX_NODES][MAX_NODES];
    int visited[MAX_NODES];
};

void initialize_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g->adjacency[i][j] = 0;
        }
        g->visited[i] = 0;
    }
}

int validate_node(struct Graph *g, int node) {
    return (node >= 0 && node < g->num_nodes);
}

int add_edge(struct Graph *g, int from, int to, int weight) {
    if (!validate_node(g, from) || !validate_node(g, to)) {
        return 0;
    }
    if (from == to) {
        return 0;
    }
    if (g->adjacency[from][to] != 0) {
        return 0;
    }
    if (g->num_edges >= MAX_EDGES) {
        return 0;
    }
    g->adjacency[from][to] = weight;
    g->adjacency[to][from] = weight;
    g->num_edges++;
    return 1;
}

void dfs(struct Graph *g, int node) {
    if (!validate_node(g, node)) {
        return;
    }
    g->visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adjacency[node][i] != 0 && !g->visited[i]) {
            dfs(g, i);
        }
    }
}

void bfs(struct Graph *g, int start) {
    if (!validate_node(g, start)) {
        return;
    }
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    for (int i = 0; i < g->num_nodes; i++) {
        g->visited[i] = 0;
    }
    g->visited[start] = 1;
    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adjacency[current][i] != 0 && !g->visited[i]) {
                g->visited[i] = 1;
                queue[rear++] = i;
            }
        }
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    int num_nodes;
    if (scanf("%d", &num_nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (num_nodes < 1 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    graph.num_nodes = num_nodes;
    printf("Enter number of edges: ");
    int num_edges;
    if (scanf("%d", &num_edges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (num_edges < 0 || num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    printf("Enter edges as 'from to weight' (nodes 0-%d):\n", num_nodes - 1);
    for (int i = 0; i < num_edges; i++) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&graph, from, to, weight)) {
            printf("Failed to add edge %d-%d\n", from, to);
            return 1;
        }
    }
    printf("DFS traversal from node 0: ");
    dfs(&graph, 0);
    printf("\n");
    printf("BFS traversal from node 0: ");
    bfs(&graph, 0);
    printf("\n");
    return 0;
}