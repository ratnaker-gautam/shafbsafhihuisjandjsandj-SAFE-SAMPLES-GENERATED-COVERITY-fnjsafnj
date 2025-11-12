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

int add_node(struct Graph *g) {
    if (g->num_nodes >= MAX_NODES) {
        return -1;
    }
    return g->num_nodes++;
}

int add_edge(struct Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->num_nodes || to < 0 || to >= g->num_nodes) {
        return -1;
    }
    if (g->adjacency[from][to] == 1) {
        return -1;
    }
    g->adjacency[from][to] = 1;
    g->edge_weights[from][to] = weight;
    g->num_edges++;
    return 0;
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

int find_min_distance(int dist[], int visited[], int n) {
    int min = -1;
    int min_val = 2147483647;
    for (int i = 0; i < n; i++) {
        if (visited[i] == 0 && dist[i] < min_val) {
            min_val = dist[i];
            min = i;
        }
    }
    return min;
}

void dijkstra(struct Graph *g, int start) {
    if (start < 0 || start >= g->num_nodes) {
        printf("Invalid start node\n");
        return;
    }
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = 2147483647;
        visited[i] = 0;
    }
    dist[start] = 0;
    for (int count = 0; count < g->num_nodes - 1; count++) {
        int u = find_min_distance(dist, visited, g->num_nodes);
        if (u == -1) {
            break;
        }
        visited[u] = 1;
        for (int v = 0; v < g->num_nodes; v++) {
            if (g->adjacency[u][v] == 1 && visited[v] == 0) {
                int new_dist = dist[u] + g->edge_weights[u][v];
                if (new_dist > 0 && new_dist < dist[v]) {
                    dist[v] = new_dist;
                }
            }
        }
    }
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < g->num_nodes; i++) {
        if (dist[i] == 2147483647) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph g;
    initialize_graph(&g);
    int num_nodes;
    printf("Enter number of nodes (max %d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1 || num_nodes <= 0 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    for (int i = 0; i < num_nodes; i++) {
        add_node(&g);
    }
    int num_edges;
    printf("Enter number of edges: ");
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    for (int i = 0; i < num_edges; i++) {
        int from, to, weight;
        printf("Enter edge %d (from to weight): ", i + 1);
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (add_edge(&g, from, to, weight) != 0) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    printf("\nGraph created with %d nodes and %d edges\n", g.num_nodes, g.num_edges);
    int visited[MAX_NODES];
    for