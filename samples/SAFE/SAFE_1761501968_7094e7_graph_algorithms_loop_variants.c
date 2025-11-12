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
        exit(1);
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weight not allowed\n");
        exit(1);
    }
    g->edges[from][to] = weight;
    g->edges[to][from] = weight;
}

int find_min_distance(int distances[], int visited[], int n) {
    int min = INT_MAX;
    int min_index = -1;
    int i = 0;
    while (i < n) {
        if (!visited[i] && distances[i] <= min) {
            min = distances[i];
            min_index = i;
        }
        i++;
    }
    return min_index;
}

void dijkstra(Graph *g, int start) {
    int distances[MAX_NODES];
    int visited[MAX_NODES];
    
    int i = 0;
    while (i < g->node_count) {
        distances[i] = INT_MAX;
        visited[i] = 0;
        i++;
    }
    distances[start] = 0;
    
    int count = 0;
    while (count < g->node_count - 1) {
        int u = find_min_distance(distances, visited, g->node_count);
        if (u == -1) break;
        visited[u] = 1;
        
        int v = 0;
        do {
            if (!visited[v] && g->edges[u][v] != 0 && 
                distances[u] != INT_MAX) {
                int new_dist = distances[u] + g->edges[u][v];
                if (new_dist > 0 && new_dist < distances[v]) {
                    distances[v] = new_dist;
                }
            }
            v++;
        } while (v < g->node_count);
        count++;
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (int j = 0; j < g->node_count; j++) {
        if (distances[j] == INT_MAX) {
            printf("Node %d: unreachable\n", j);
        } else {
            printf("Node %d: %d\n", j, distances[j]);
        }
    }
}

int main() {
    Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
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
    
    printf("Enter edges (from to weight):\n");
    int edge_count = 0;
    while (edge_count < edges) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&g, from, to, weight);
        edge_count++;
    }
    
    int start_node;
    printf("Enter starting node: ");
    if (scanf("%d", &start_node) != 1 || start_node < 0 || start_node >= nodes) {
        fprintf(stderr, "Invalid starting node\n");
        return 1;
    }
    
    dijkstra(&g, start_node);
    
    return 0;
}