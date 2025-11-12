//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
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
        fprintf(stderr, "Invalid node count\n");
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
        fprintf(stderr, "Invalid edge nodes\n");
        exit(1);
    }
    g->adjacency[from][to] = weight;
    g->adjacency[to][from] = weight;
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

void dijkstra_shortest_path(Graph *g, int start) {
    int distances[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        distances[i] = INT_MAX;
        visited[i] = 0;
    }
    distances[start] = 0;
    
    int count = 0;
    while (count < g->node_count - 1) {
        int current = find_min_distance(distances, visited, g->node_count);
        if (current == -1) break;
        visited[current] = 1;
        
        int neighbor = 0;
        do {
            if (!visited[neighbor] && g->adjacency[current][neighbor] != 0) {
                long new_dist = (long)distances[current] + g->adjacency[current][neighbor];
                if (new_dist < distances[neighbor] && new_dist <= INT_MAX) {
                    distances[neighbor] = (int)new_dist;
                }
            }
            neighbor++;
        } while (neighbor < g->node_count);
        count++;
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
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initialize_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Enter edges as 'from to weight' (0-indexed):\n");
    for (int i = 0; i < edges; i++) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (from < 0 || from >= nodes || to < 0 || to >= nodes || weight < 0) {
            fprintf(stderr, "Invalid edge parameters\n");
            return 1;
        }
        add_edge(&g, from, to, weight);
    }
    
    int start_node;
    printf("Enter starting node: ");
    if (scanf("%d", &start_node) != 1 || start_node < 0 || start_node >= nodes) {
        fprintf(stderr, "Invalid start node\n");
        return 1;
    }
    
    dijkstra_shortest_path(&g, start_node);
    
    return 0;
}