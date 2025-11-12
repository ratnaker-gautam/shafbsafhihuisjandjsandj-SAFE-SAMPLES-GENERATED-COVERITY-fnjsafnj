//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 5000

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
    if (weight < -1000 || weight > 1000) {
        return 0;
    }
    g->adjacency[from][to] = 1;
    g->edge_weights[from][to] = weight;
    g->num_edges++;
    return 1;
}

void floyd_warshall(struct Graph *g, int distances[MAX_NODES][MAX_NODES]) {
    for (int i = 0; i < g->num_nodes; i++) {
        for (int j = 0; j < g->num_nodes; j++) {
            if (i == j) {
                distances[i][j] = 0;
            } else if (g->adjacency[i][j]) {
                distances[i][j] = g->edge_weights[i][j];
            } else {
                distances[i][j] = INT_MAX / 2;
            }
        }
    }

    for (int k = 0; k < g->num_nodes; k++) {
        for (int i = 0; i < g->num_nodes; i++) {
            for (int j = 0; j < g->num_nodes; j++) {
                if (distances[i][k] < INT_MAX / 2 && distances[k][j] < INT_MAX / 2) {
                    if (distances[i][k] + distances[k][j] < distances[i][j]) {
                        distances[i][j] = distances[i][k] + distances[k][j];
                    }
                }
            }
        }
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &graph.num_nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (graph.num_nodes < 1 || graph.num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }

    printf("Enter number of edges: ");
    int edge_count;
    if (scanf("%d", &edge_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (edge_count < 0 || edge_count > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }

    printf("Enter edges as 'from to weight' (nodes 0-%d):\n", graph.num_nodes - 1);
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

    int distances[MAX_NODES][MAX_NODES];
    floyd_warshall(&graph, distances);

    printf("\nShortest path distances:\n");
    for (int i = 0; i < graph.num_nodes; i++) {
        for (int j = 0; j < graph.num_nodes; j++) {
            if (i != j) {
                if (distances[i][j] > INT_MAX / 4) {
                    printf("Node %d to %d: No path\n", i, j);
                } else {
                    printf("Node %d to %d: %d\n", i, j, distances[i][j]);
                }
            }
        }
    }

    printf("\nPath queries (enter -1 to exit):\n");
    while (1) {
        int source, target;
        printf("Enter source and target nodes: ");
        if (scanf("%d %d", &source, &target) != 2) {
            printf("Invalid input\n");
            break;
        }
        if (source == -1 || target == -1) {
            break;
        }
        if (!validate_node(&graph, source) || !validate_node(&graph, target)) {
            printf("Invalid