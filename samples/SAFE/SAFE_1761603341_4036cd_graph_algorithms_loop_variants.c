//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int edges[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

void initialize_graph(Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    g->node_count = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->edges[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid edge nodes\n");
        return;
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weight not allowed\n");
        return;
    }
    g->edges[from][to] = weight;
}

void dijkstra_shortest_path(Graph *g, int source) {
    if (source < 0 || source >= g->node_count) {
        fprintf(stderr, "Invalid source node\n");
        return;
    }

    int distances[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        distances[i] = INT_MAX;
        visited[i] = 0;
    }
    distances[source] = 0;

    for (int count = 0; count < g->node_count - 1; count++) {
        int min_distance = INT_MAX;
        int min_index = -1;

        int i = 0;
        while (i < g->node_count) {
            if (!visited[i] && distances[i] <= min_distance) {
                min_distance = distances[i];
                min_index = i;
            }
            i++;
        }

        if (min_index == -1) break;

        visited[min_index] = 1;

        int j = 0;
        do {
            if (!visited[j] && g->edges[min_index][j] != 0) {
                long new_dist = (long)distances[min_index] + g->edges[min_index][j];
                if (new_dist < distances[j] && new_dist <= INT_MAX) {
                    distances[j] = (int)new_dist;
                }
            }
            j++;
        } while (j < g->node_count);
    }

    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < g->node_count; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

int main() {
    Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid input for nodes\n");
        return 1;
    }
    
    initialize_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        fprintf(stderr, "Invalid input for edges\n");
        return 1;
    }
    
    printf("Enter edges as 'from to weight' (0-indexed):\n");
    for (int i = 0; i < edges; i++) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&g, from, to, weight);
    }
    
    int source;
    printf("Enter source node for Dijkstra: ");
    if (scanf("%d", &source) != 1 || source < 0 || source >= nodes) {
        fprintf(stderr, "Invalid source node\n");
        return 1;
    }
    
    dijkstra_shortest_path(&g, source);
    
    return 0;
}