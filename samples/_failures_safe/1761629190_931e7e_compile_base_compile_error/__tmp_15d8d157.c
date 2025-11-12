//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int num_nodes;
    int num_edges;
    int adjacency[MAX_NODES][MAX_NODES];
    char node_names[MAX_NODES][32];
};

void initialize_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g->adjacency[i][j] = 0;
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
    if (g->num_edges >= MAX_EDGES) {
        return -1;
    }
    if (weight < 0) {
        return -1;
    }
    
    g->adjacency[from_idx][to_idx] = weight;
    g->num_edges++;
    return 0;
}

void depth_first_search(struct Graph *g, int node, int *visited) {
    if (node < 0 || node >= g->num_nodes) {
        return;
    }
    if (visited[node]) {
        return;
    }
    
    visited[node] = 1;
    printf("Visited: %s\n", g->node_names[node]);
    
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adjacency[node][i] > 0 && !visited[i]) {
            depth_first_search(g, i, visited);
        }
    }
}

void perform_dfs(struct Graph *g, const char *start_node) {
    int start_idx = find_node_index(g, start_node);
    if (start_idx == -1) {
        printf("Start node not found\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal starting from %s:\n", start_node);
    depth_first_search(g, start_idx, visited);
}

void display_graph(struct Graph *g) {
    printf("Graph has %d nodes and %d edges\n", g->num_nodes, g->num_edges);
    printf("Nodes: ");
    for (int i = 0; i < g->num_nodes; i++) {
        printf("%s", g->node_names[i]);
        if (i < g->num_nodes - 1) {
            printf(", ");
        }
    }
    printf("\n");
    
    printf("Edges:\n");
    for (int i = 0; i < g->num_nodes; i++) {
        for (int j = 0; j < g->num_nodes; j++) {
            if (g->adjacency[i][j] > 0) {
                printf("  %s -> %s (weight: %d)\n", 
                       g->node_names[i], g->node_names[j], g->adjacency[i][j]);
            }
        }
    }
}

int main() {
    struct Graph g;
    initialize_graph(&g);
    
    char input[256];
    char node1[32], node2[32];
    int weight;
    
    printf("Graph Construction and DFS Demo\n");
    printf("Available commands:\n");
    printf("  add_node <name>\n");
    printf("  add_edge <from> <to> <weight>\n");
    printf("  dfs <start_node>\n");
    printf("  display\n");
    printf("  exit\n\n");
    
    while (1) {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strncmp(input, "add_node", 8) == 0) {
            if (sscanf(input + 8, " %31s", node1) == 1) {
                if (add_node(&g, node1) == -1) {
                    printf("Failed to add node\n");
                } else {
                    printf("Added node: %s\n", node1);
                }
            } else {
                printf("Invalid add_node command\n");
            }
        }
        else if (strncmp(input, "add_edge", 8) == 0) {
            if