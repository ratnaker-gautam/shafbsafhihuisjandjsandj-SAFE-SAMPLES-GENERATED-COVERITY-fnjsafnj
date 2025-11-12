//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int num_nodes;
    int num_edges;
    int adjacency[MAX_NODES][MAX_NODES];
    int edge_list[MAX_EDGES][2];
};

void initialize_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

int add_node(struct Graph *g) {
    if (g->num_nodes >= MAX_NODES) {
        return -1;
    }
    return g->num_nodes++;
}

int add_edge(struct Graph *g, int u, int v) {
    if (u < 0 || u >= g->num_nodes || v < 0 || v >= g->num_nodes) {
        return 0;
    }
    if (g->num_edges >= MAX_EDGES) {
        return 0;
    }
    if (g->adjacency[u][v] == 1) {
        return 0;
    }
    g->adjacency[u][v] = 1;
    g->adjacency[v][u] = 1;
    g->edge_list[g->num_edges][0] = u;
    g->edge_list[g->num_edges][1] = v;
    g->num_edges++;
    return 1;
}

void depth_first_search(struct Graph *g, int start, int visited[]) {
    if (start < 0 || start >= g->num_nodes) {
        return;
    }
    visited[start] = 1;
    printf("%d ", start);
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adjacency[start][i] == 1 && visited[i] == 0) {
            depth_first_search(g, i, visited);
        }
    }
}

void breadth_first_search(struct Graph *g, int start) {
    if (start < 0 || start >= g->num_nodes) {
        return;
    }
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adjacency[current][i] == 1 && visited[i] == 0) {
                visited[i] = 1;
                queue[rear++] = i;
            }
        }
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    int num_nodes, num_edges;
    printf("Enter number of nodes (max %d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1 || num_nodes <= 0 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    for (int i = 0; i < num_nodes; i++) {
        add_node(&graph);
    }
    printf("Enter number of edges (max %d): ", MAX_EDGES);
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    printf("Enter edges as pairs of node indices (0 to %d):\n", num_nodes - 1);
    for (int i = 0; i < num_edges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&graph, u, v)) {
            printf("Failed to add edge %d-%d\n", u, v);
            return 1;
        }
    }
    printf("Graph created with %d nodes and %d edges\n", graph.num_nodes, graph.num_edges);
    printf("DFS traversal: ");
    int visited[MAX_NODES] = {0};
    depth_first_search(&graph, 0, visited);
    printf("\n");
    printf("BFS traversal: ");
    breadth_first_search(&graph, 0);
    printf("\n");
    return 0;
}