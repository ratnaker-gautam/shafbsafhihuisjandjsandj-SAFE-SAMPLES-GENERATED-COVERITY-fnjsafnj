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
    int from_idx = find_node_index(g, from);
    int to_idx = find_node_index(g, to);
    
    if (from_idx == -1 || to_idx == -1) {
        return -1;
    }
    if (from_idx == to_idx) {
        return -1;
    }
    if (weight < 0) {
        return -1;
    }
    
    if (g->adj_matrix[from_idx][to_idx] == 0) {
        g->num_edges++;
    }
    g->adj_matrix[from_idx][to_idx] = weight;
    return 0;
}

void depth_first_search(struct Graph *g, int node, int visited[]) {
    if (node < 0 || node >= g->num_nodes) {
        return;
    }
    visited[node] = 1;
    printf("%s ", g->node_names[node]);
    
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adj_matrix[node][i] != 0 && !visited[i]) {
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

void print_graph(struct Graph *g) {
    printf("Graph has %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    for (int i = 0; i < g->num_nodes; i++) {
        printf("Node %d: %s\n", i, g->node_names[i]);
    }
    printf("Edges:\n");
    for (int i = 0; i < g->num_nodes; i++) {
        for (int j = 0; j < g->num_nodes; j++) {
            if (g->adj_matrix[i][j] != 0) {
                printf("  %s -> %s (weight: %d)\n", 
                       g->node_names[i], g->node_names[j], g->adj_matrix[i][j]);
            }
        }
    }
}

int main() {
    struct Graph g;
    initialize_graph(&g);
    
    printf("Graph Algorithms Demo\n");
    printf("Adding nodes...\n");
    
    add_node(&g, "A");
    add_node(&g, "B");
    add_node(&g, "C");
    add_node(&g, "D");
    add_node(&g, "E");
    
    printf("Adding edges...\n");
    add_edge(&g, "A", "B", 5);
    add_edge(&g, "A", "C", 3);
    add_edge(&g, "B", "D", 2);
    add_edge(&g, "C", "D", 1);
    add_edge(&g, "D", "E", 4);
    add_edge(&g, "E", "A", 6);
    
    print_graph(&g);
    printf("\n");
    
    printf("Enter starting node for DFS (A-E): ");
    char start_node[32];
    if (fgets(start_node, sizeof(start_node), stdin) != NULL) {
        size_t len = strlen(start_node);
        if (len > 0 && start_node[len-1] == '\n') {
            start_node[len-1] = '\0';
        }
        perform_dfs(&g, start_node);
    }
    
    printf("Testing with invalid node...\n");
    perform_dfs(&g, "X");
    
    printf("Testing edge cases