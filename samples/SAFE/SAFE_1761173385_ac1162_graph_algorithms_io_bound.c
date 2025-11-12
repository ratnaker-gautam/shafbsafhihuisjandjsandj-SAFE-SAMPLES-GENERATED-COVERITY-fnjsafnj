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
    for (int i = 0; i < g->num_nodes; i++) {
        g->visited[i] = 0;
    }
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    queue[rear++] = start;
    g->visited[start] = 1;
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adjacency[current][i] != 0 && !g->visited[i]) {
                queue[rear++] = i;
                g->visited[i] = 1;
                if (rear >= MAX_NODES) {
                    rear = MAX_NODES - 1;
                }
            }
        }
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    int node_count;
    if (scanf("%d", &node_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (node_count < 1 || node_count > MAX_NODES) {
        printf("Invalid node count\n");
        return 1;
    }
    graph.num_nodes = node_count;
    printf("Enter number of edges: ");
    int edge_count;
    if (scanf("%d", &edge_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (edge_count < 0 || edge_count > MAX_EDGES) {
        printf("Invalid edge count\n");
        return 1;
    }
    printf("Enter edges as 'from to weight' (0-based indices):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid edge format\n");
            return 1;
        }
        if (!add_edge(&graph, from, to, weight)) {
            printf("Invalid edge: %d %d %d\n", from, to, weight);
            return 1;
        }
    }
    printf("Graph created with %d nodes and %d edges\n", graph.num_nodes, graph.num_edges);
    printf("Enter starting node for DFS: ");
    int start_node;
    if (scanf("%d", &start_node) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (!validate_node(&graph, start_node)) {
        printf("Invalid starting node\n");
        return 1;
    }
    printf("DFS traversal: ");
    for (int i = 0; i < graph.num_nodes; i++) {
        graph.visited[i] = 0;
    }
    dfs(&graph, start_node);
    printf("\n");
    printf("Enter starting node for BFS: ");
    if (scanf("%d", &start_node) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (!validate_node(&graph, start_node)) {
        printf("Invalid starting node\n");
        return 1;
    }
    printf("BFS traversal: ");
    bfs(&graph, start_node);
    printf("\n");
    return 0;
}