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
    if (weight <= 0) {
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

void depth_first_search(struct Graph *g, int start, int visited[]) {
    if (start < 0 || start >= g->num_nodes) {
        return;
    }
    visited[start] = 1;
    printf("%s ", g->node_names[start]);
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adj_matrix[start][i] != 0 && !visited[i]) {
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

void display_graph(struct Graph *g) {
    printf("Graph has %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    for (int i = 0; i < g->num_nodes; i++) {
        printf("Node %d: %s\n", i, g->node_names[i]);
    }
    printf("Edges:\n");
    for (int i = 0; i < g->num_nodes; i++) {
        for (int j = 0; j < g->num_nodes; j++) {
            if (g->adj_matrix[i][j] != 0) {
                printf("%s -> %s (weight: %d)\n", g->node_names[i], g->node_names[j], g->adj_matrix[i][j]);
            }
        }
    }
}

int main() {
    struct Graph g;
    initialize_graph(&g);
    
    printf("Graph Construction and DFS Traversal\n");
    printf("Enter commands: add_node <name>, add_edge <from> <to> <weight>, dfs <start>, display, exit\n");
    
    char command[64];
    char arg1[32], arg2[32];
    int weight;
    
    while (1) {
        printf("> ");
        if (scanf("%63s", command) != 1) {
            break;
        }
        
        if (strcmp(command, "add_node") == 0) {
            if (scanf("%31s", arg1) != 1) {
                printf("Invalid input.\n");
                while (getchar() != '\n');
                continue;
            }
            if (add_node(&g, arg1) == -1) {
                printf("Failed to add node.\n");
            } else {
                printf("Added node: %s\n", arg1);
            }
        } else if (strcmp(command, "add_edge") == 0) {
            if (scanf("%31s %31s %d", arg1, arg2, &weight) != 3) {
                printf("Invalid input.\n");
                while (getchar() != '\n');
                continue;
            }
            if (add_edge(&g, arg1, arg2, weight) == -1) {
                printf("Failed to add edge.\n");
            } else {
                printf("Added edge: %s -> %s (weight: %