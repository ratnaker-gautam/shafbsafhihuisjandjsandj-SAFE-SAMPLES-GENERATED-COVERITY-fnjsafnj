//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int num_nodes;
    int num_edges;
    int adj_matrix[MAX_NODES][MAX_NODES];
    char node_names[MAX_NODES][32];
};

void initialize_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g->adj_matrix[i][j] = 0;
        }
        g->node_names[i][0] = '\0';
    }
}

int find_node_index(struct Graph *g, const char *name) {
    for (int i = 0; i < g->num_nodes; i++) {
        if (strcmp(g->node_names[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

int add_node(struct Graph *g, const char *name) {
    if (g->num_nodes >= MAX_NODES) {
        return -1;
    }
    if (strlen(name) >= 32) {
        return -1;
    }
    if (find_node_index(g, name) != -1) {
        return -1;
    }
    strcpy(g->node_names[g->num_nodes], name);
    g->num_nodes++;
    return g->num_nodes - 1;
}

int add_edge(struct Graph *g, const char *from, const char *to, int weight) {
    if (weight < 0) {
        return -1;
    }
    int from_idx = find_node_index(g, from);
    int to_idx = find_node_index(g, to);
    if (from_idx == -1 || to_idx == -1) {
        return -1;
    }
    if (g->adj_matrix[from_idx][to_idx] != 0) {
        return -1;
    }
    g->adj_matrix[from_idx][to_idx] = weight;
    g->num_edges++;
    return 0;
}

void depth_first_search(struct Graph *g, int start_idx, int *visited) {
    if (visited[start_idx]) {
        return;
    }
    visited[start_idx] = 1;
    printf("%s ", g->node_names[start_idx]);
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adj_matrix[start_idx][i] != 0 && !visited[i]) {
            depth_first_search(g, i, visited);
        }
    }
}

void perform_dfs(struct Graph *g, const char *start_node) {
    int start_idx = find_node_index(g, start_node);
    if (start_idx == -1) {
        printf("Start node not found.\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from %s: ", start_node);
    depth_first_search(g, start_idx, visited);
    printf("\n");
}

void find_shortest_path(struct Graph *g, const char *start_node, const char *end_node) {
    int start_idx = find_node_index(g, start_node);
    int end_idx = find_node_index(g, end_node);
    if (start_idx == -1 || end_idx == -1) {
        printf("Invalid nodes specified.\n");
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    int prev[MAX_NODES];
    
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = -1;
        prev[i] = -1;
    }
    dist[start_idx] = 0;
    
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
            if (g->adj_matrix[min_node][i] != 0 && !visited[i]) {
                int new_dist = dist[min_node] + g->adj_matrix[min_node][i];
                if (dist[i] == -1 || new_dist < dist[i]) {
                    dist[i] = new_dist;
                    prev[i] = min_node;
                }
            }
        }
    }
    
    if (dist[end_idx] == -1) {
        printf("No path exists between %s and %s.\n", start_node, end_node);
        return;
    }
    
    printf("Shortest path from %s to %s: ", start_node, end_node);
    int path[MAX_NODES];
    int path_len = 0;
    int current = end_idx;
    
    while (current != -1) {
        path[path_len++] = current;
        current = prev[current];
    }
    
    for (int i = path_len - 1; i >= 0; i--) {