//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

void initialize_graph(Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->node_count = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        exit(1);
    }
    g->adjacency[from][to] = weight;
    g->adjacency[to][from] = weight;
}

int find_min_distance(int distances[], int visited[], int n) {
    int min = INT_MAX;
    int min_index = -1;
    for (int v = 0; v < n; v++) {
        if (visited[v] == 0 && distances[v] <= min) {
            min = distances[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(Graph *g, int start) {
    int distances[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        distances[i] = INT_MAX;
        visited[i] = 0;
    }
    distances[start] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int u = find_min_distance(distances, visited, g->node_count);
        if (u == -1) break;
        visited[u] = 1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && g->adjacency[u][v] != 0) {
                long new_dist = (long)distances[u] + g->adjacency[u][v];
                if (new_dist < distances[v] && new_dist <= INT_MAX) {
                    distances[v] = (int)new_dist;
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", start);
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
    if (scanf("%d", &nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initialize_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (edges < 0 || edges > nodes * (nodes - 1) / 2) {
        fprintf(stderr, "Invalid number of edges\n");
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
    
    int start_node;
    printf("Enter starting node for Dijkstra: ");
    if (scanf("%d", &start_node) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (start_node < 0 || start_node >= nodes) {
        fprintf(stderr, "Invalid starting node\n");
        return 1;
    }
    
    dijkstra(&g, start_node);
    
    return 0;
}