//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct Graph {
    int nodes;
    int adj[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph* g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        nodes = MAX_NODES;
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
}

int validate_node(struct Graph* g, int node) {
    return (node >= 0 && node < g->nodes);
}

void add_edge(struct Graph* g, int u, int v, int weight) {
    if (!validate_node(g, u) || !validate_node(g, v)) {
        return;
    }
    if (weight < 0 || weight > 10000) {
        return;
    }
    g->adj[u][v] = weight;
    g->adj[v][u] = weight;
}

void floyd_warshall(struct Graph* g, int dist[MAX_NODES][MAX_NODES]) {
    for (int i = 0; i < g->nodes; i++) {
        for (int j = 0; j < g->nodes; j++) {
            dist[i][j] = g->adj[i][j];
        }
    }
    
    for (int k = 0; k < g->nodes; k++) {
        for (int i = 0; i < g->nodes; i++) {
            for (int j = 0; j < g->nodes; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    long sum = (long)dist[i][k] + dist[k][j];
                    if (sum < dist[i][j] && sum <= INF) {
                        dist[i][j] = (int)sum;
                    }
                }
            }
        }
    }
}

void print_shortest_paths(struct Graph* g, int dist[MAX_NODES][MAX_NODES]) {
    printf("Shortest path distances:\n");
    for (int i = 0; i < g->nodes; i++) {
        for (int j = 0; j < g->nodes; j++) {
            if (dist[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", dist[i][j]);
            }
        }
        printf("\n");
    }
}

int get_valid_int(const char* prompt, int min_val, int max_val) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min_val, max_val);
    }
}

int main() {
    struct Graph graph;
    int dist[MAX_NODES][MAX_NODES];
    
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n");
    
    int nodes = get_valid_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    initialize_graph(&graph, nodes);
    
    int edges = get_valid_int("Enter number of edges: ", 0, nodes * (nodes - 1) / 2);
    
    printf("Enter edges as: source destination weight\n");
    printf("Nodes are numbered 0 to %d\n", nodes - 1);
    
    for (int i = 0; i < edges; i++) {
        printf("Edge %d: ", i + 1);
        
        int u, v, w;
        char buffer[100];
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            i--;
            continue;
        }
        
        if (sscanf(buffer, "%d %d %d", &u, &v, &w) != 3) {
            printf("Invalid format. Please enter three numbers.\n");
            i--;
            continue;
        }
        
        if (!validate_node(&graph, u) || !validate_node(&graph, v)) {
            printf("Invalid node numbers. Must be between 0 and %d.\n", nodes - 1);
            i--;
            continue;
        }
        
        if (w < 0 || w > 10000) {
            printf("Weight must be between 0 and 10000.\n");
            i--;
            continue;
        }
        
        add_edge(&graph, u, v, w);
    }
    
    floyd_warshall(&graph, dist);
    print_shortest_paths(&graph, dist);
    
    printf("\nQuery shortest paths between nodes:\n");
    while (1) {
        int src = get_valid_int("Enter source node (-1 to exit): ", -1, nodes - 1);
        if (src == -1) break;
        
        int dest = get_valid_int("Enter destination node: ", 0, nodes - 1);
        
        if (dist[src][dest] == INF) {
            printf("