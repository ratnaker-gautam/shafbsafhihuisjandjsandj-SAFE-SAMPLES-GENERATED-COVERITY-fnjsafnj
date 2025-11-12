//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000
#define MAX_LINE 256

typedef struct {
    int from;
    int to;
    int weight;
} Edge;

typedef struct {
    int node_count;
    int edge_count;
    Edge edges[MAX_EDGES];
    int adjacency[MAX_NODES][MAX_NODES];
} Graph;

void initialize_graph(Graph *g) {
    g->node_count = 0;
    g->edge_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

int add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= MAX_NODES || to < 0 || to >= MAX_NODES) {
        return 0;
    }
    if (g->edge_count >= MAX_EDGES) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    
    g->edges[g->edge_count].from = from;
    g->edges[g->edge_count].to = to;
    g->edges[g->edge_count].weight = weight;
    g->edge_count++;
    
    g->adjacency[from][to] = weight;
    g->adjacency[to][from] = weight;
    
    if (from > g->node_count - 1) g->node_count = from + 1;
    if (to > g->node_count - 1) g->node_count = to + 1;
    
    return 1;
}

void dijkstra(Graph *g, int start, int distances[]) {
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->node_count; i++) {
        distances[i] = -1;
    }
    distances[start] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_dist = -1;
        int min_node = -1;
        
        for (int i = 0; i < g->node_count; i++) {
            if (!visited[i] && distances[i] != -1 && (min_dist == -1 || distances[i] < min_dist)) {
                min_dist = distances[i];
                min_node = i;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int i = 0; i < g->node_count; i++) {
            if (!visited[i] && g->adjacency[min_node][i] != 0) {
                int new_dist = distances[min_node] + g->adjacency[min_node][i];
                if (distances[i] == -1 || new_dist < distances[i]) {
                    distances[i] = new_dist;
                }
            }
        }
    }
}

int read_graph_from_input(Graph *g) {
    char line[MAX_LINE];
    int node_count;
    
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    if (sscanf(line, "%d", &node_count) != 1) return 0;
    if (node_count <= 0 || node_count > MAX_NODES) return 0;
    
    initialize_graph(g);
    
    for (int i = 0; i < node_count; i++) {
        if (fgets(line, sizeof(line), stdin) == NULL) return 0;
        
        char *token = strtok(line, " ");
        int token_count = 0;
        int weights[MAX_NODES];
        
        while (token != NULL && token_count < node_count) {
            if (sscanf(token, "%d", &weights[token_count]) != 1) return 0;
            token = strtok(NULL, " ");
            token_count++;
        }
        
        if (token_count != node_count) return 0;
        
        for (int j = 0; j < node_count; j++) {
            if (weights[j] > 0) {
                if (!add_edge(g, i, j, weights[j])) return 0;
            }
        }
    }
    
    return 1;
}

void print_shortest_paths(Graph *g, int start) {
    int distances[MAX_NODES];
    dijkstra(g, start, distances);
    
    printf("Shortest paths from node %d:\n", start);
    for (int i = 0; i < g->node_count; i++) {
        if (distances[i] == -1) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

int main() {
    Graph g;
    
    printf("Enter number of nodes: ");
    
    if (!read_graph_from_input(&g)) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Graph loaded with %d nodes and %d edges\n", g.node_count, g.edge_count);
    
    int start_node;
    printf("Enter starting node for shortest paths: ");
    if (scanf("%d", &start_node) != 1) {
        printf