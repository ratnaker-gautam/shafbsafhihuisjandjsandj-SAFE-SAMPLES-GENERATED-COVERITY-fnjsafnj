//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 500

struct Graph {
    int num_nodes;
    int num_edges;
    int adjacency[MAX_NODES][MAX_NODES];
    int edge_weights[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g->adjacency[i][j] = 0;
            g->edge_weights[i][j] = 0;
        }
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
    if (g->adjacency[from][to] == 1) {
        return 0;
    }
    if (g->num_edges >= MAX_EDGES) {
        return 0;
    }
    g->adjacency[from][to] = 1;
    g->edge_weights[from][to] = weight;
    g->num_edges++;
    return 1;
}

void depth_first_search(struct Graph *g, int start, int visited[]) {
    if (!validate_node(g, start)) {
        return;
    }
    visited[start] = 1;
    printf("%d ", start);
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adjacency[start][i] && !visited[i]) {
            depth_first_search(g, i, visited);
        }
    }
}

void perform_dfs(struct Graph *g) {
    int visited[MAX_NODES] = {0};
    int start_node;
    printf("Enter starting node for DFS (0-%d): ", g->num_nodes - 1);
    if (scanf("%d", &start_node) != 1) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        return;
    }
    if (!validate_node(g, start_node)) {
        printf("Invalid node\n");
        return;
    }
    printf("DFS traversal: ");
    depth_first_search(g, start_node, visited);
    printf("\n");
}

void find_shortest_path(struct Graph *g) {
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    int start, end;
    printf("Enter start and end nodes (0-%d): ", g->num_nodes - 1);
    if (scanf("%d %d", &start, &end) != 2) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        return;
    }
    if (!validate_node(g, start) || !validate_node(g, end)) {
        printf("Invalid nodes\n");
        return;
    }
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = -1;
    }
    dist[start] = 0;
    for (int count = 0; count < g->num_nodes - 1; count++) {
        int min_dist = -1;
        int min_node = -1;
        for (int i = 0; i < g->num_nodes; i++) {
            if (!visited[i] && dist[i] != -1 && (min_dist == -1 || dist[i] < min_dist)) {
                min_dist = dist[i];
                min_node = i;
            }
        }
        if (min_node == -1) break;
        visited[min_node] = 1;
        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adjacency[min_node][i] && !visited[i]) {
                int new_dist = dist[min_node] + g->edge_weights[min_node][i];
                if (dist[i] == -1 || new_dist < dist[i]) {
                    dist[i] = new_dist;
                }
            }
        }
    }
    if (dist[end] == -1) {
        printf("No path exists between %d and %d\n", start, end);
    } else {
        printf("Shortest path distance: %d\n", dist[end]);
    }
}

void print_graph(struct Graph *g) {
    printf("Graph (%d nodes, %d edges):\n", g->num_nodes, g->num_edges);
    for (int i = 0; i < g->num_nodes; i++) {
        printf("Node %d: ", i);
        int has_edges = 0;
        for (int j = 0; j < g->num_nodes; j++) {
            if (g->adjacency[i][j]) {
                printf("%d(w:%d) ", j, g->edge_weights[i][j]);
                has_edges = 1;
            }
        }
        if (!has_edges) {
            printf("no edges");
        }
        printf("\n");
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    int choice;
    printf("Enter number of nodes (1-%