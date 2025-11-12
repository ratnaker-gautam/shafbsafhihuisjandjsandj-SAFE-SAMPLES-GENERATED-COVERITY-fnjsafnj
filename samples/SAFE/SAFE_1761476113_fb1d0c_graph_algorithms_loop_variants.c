//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Edge {
    int src;
    int dest;
    int weight;
};

struct Graph {
    int num_nodes;
    int num_edges;
    struct Edge edges[MAX_EDGES];
};

void initialize_graph(struct Graph* graph, int nodes, int edges) {
    if (nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    graph->num_nodes = nodes;
    graph->num_edges = edges;
}

int validate_edge(int src, int dest, int weight, int num_nodes) {
    if (src < 0 || src >= num_nodes || dest < 0 || dest >= num_nodes) {
        return 0;
    }
    if (weight < -1000 || weight > 1000) {
        return 0;
    }
    return 1;
}

void add_edge(struct Graph* graph, int index, int src, int dest, int weight) {
    if (index < 0 || index >= graph->num_edges) {
        fprintf(stderr, "Invalid edge index\n");
        exit(1);
    }
    if (!validate_edge(src, dest, weight, graph->num_nodes)) {
        fprintf(stderr, "Invalid edge parameters\n");
        exit(1);
    }
    graph->edges[index].src = src;
    graph->edges[index].dest = dest;
    graph->edges[index].weight = weight;
}

void bellman_ford(struct Graph* graph, int start) {
    if (start < 0 || start >= graph->num_nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }

    int distances[MAX_NODES];
    int i = 0;
    while (i < graph->num_nodes) {
        distances[i] = INT_MAX;
        i++;
    }
    distances[start] = 0;

    int j;
    for (j = 0; j < graph->num_nodes - 1; j++) {
        int k = 0;
        while (k < graph->num_edges) {
            int u = graph->edges[k].src;
            int v = graph->edges[k].dest;
            int weight = graph->edges[k].weight;
            if (distances[u] != INT_MAX) {
                long long new_dist = (long long)distances[u] + weight;
                if (new_dist < distances[v] && new_dist < INT_MAX) {
                    distances[v] = (int)new_dist;
                }
            }
            k++;
        }
    }

    int l = 0;
    do {
        if (l < graph->num_edges) {
            int u = graph->edges[l].src;
            int v = graph->edges[l].dest;
            int weight = graph->edges[l].weight;
            if (distances[u] != INT_MAX && (long long)distances[u] + weight < distances[v]) {
                fprintf(stderr, "Graph contains negative weight cycle\n");
                return;
            }
        }
        l++;
    } while (l < graph->num_edges);

    printf("Shortest distances from node %d:\n", start);
    int m;
    for (m = 0; m < graph->num_nodes; m++) {
        if (distances[m] == INT_MAX) {
            printf("Node %d: unreachable\n", m);
        } else {
            printf("Node %d: %d\n", m, distances[m]);
        }
    }
}

int main() {
    struct Graph graph;
    int num_nodes, num_edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &num_nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Enter number of edges (0-1000): ");
    if (scanf("%d", &num_edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initialize_graph(&graph, num_nodes, num_edges);
    
    int i = 0;
    while (i < num_edges) {
        int src, dest, weight;
        printf("Enter edge %d (src dest weight): ", i);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&graph, i, src, dest, weight);
        i++;
    }
    
    int start_node;
    printf("Enter start node: ");
    if (scanf("%d", &start_node) != 1) {
        fprintf(stderr, "Invalid start node\n");
        return 1;
    }
    
    bellman_ford(&graph, start_node);
    
    return 0;
}