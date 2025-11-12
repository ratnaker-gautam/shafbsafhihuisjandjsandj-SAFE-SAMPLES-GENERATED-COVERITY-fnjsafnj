//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 500

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
    if (weight < 0 || weight > 1000) {
        return -1;
    }
    int from_idx = find_node_index(g, from);
    int to_idx = find_node_index(g, to);
    if (from_idx == -1 || to_idx == -1) {
        return -1;
    }
    if (g->adjacency[from_idx][to_idx] != 0) {
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
    printf("%s ", g->node_names[node]);
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adjacency[node][i] != 0 && !visited[i]) {
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
        printf("Node %s connects to: ", g->node_names[i]);
        int has_edges = 0;
        for (int j = 0; j < g->num_nodes; j++) {
            if (g->adjacency[i][j] != 0) {
                printf("%s(%d) ", g->node_names[j], g->adjacency[i][j]);
                has_edges = 1;
            }
        }
        if (!has_edges) {
            printf("none");
        }
        printf("\n");
    }
}

int main() {
    struct Graph g;
    initialize_graph(&g);
    
    char input[256];
    int choice;
    
    while (1) {
        printf("\nGraph Operations:\n");
        printf("1. Add node\n");
        printf("2. Add edge\n");
        printf("3. Display graph\n");
        printf("4. Perform DFS\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (sscanf(input, "%d", &choice) != 1) {
            printf("Invalid input.\n");
            continue;
        }
        
        switch (choice) {
            case 1: {
                printf("Enter node name: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    break;
                }
                input[strcspn(input, "\n")] = '\0';
                if (add_node(&g, input) == -1) {
                    printf("Failed to add node.\n");
                } else {
                    printf("Node added successfully.\n");
                }
                break;
            }
            case 2: {
                char from[32], to[32];
                int weight;
                printf("Enter source node: ");
                if (fgets(from, sizeof(from), stdin) == NULL) break;
                from[strcspn(from, "\n")] = '\0';
                printf("Enter target node: ");
                if (