//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 500

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

void dfs_util(struct Graph *g, int node, int visited[]) {
    if (node < 0 || node >= g->num_nodes) {
        return;
    }
    visited[node] = 1;
    printf("%s ", g->node_names[node]);
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adj_matrix[node][i] != 0 && !visited[i]) {
            dfs_util(g, i, visited);
        }
    }
}

void depth_first_search(struct Graph *g, const char *start) {
    int start_idx = find_node_index(g, start);
    if (start_idx == -1) {
        printf("Start node not found\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from %s: ", start);
    dfs_util(g, start_idx, visited);
    printf("\n");
}

void breadth_first_search(struct Graph *g, const char *start) {
    int start_idx = find_node_index(g, start);
    if (start_idx == -1) {
        printf("Start node not found\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start_idx] = 1;
    queue[rear++] = start_idx;
    printf("BFS traversal from %s: ", start);
    while (front < rear) {
        int current = queue[front++];
        printf("%s ", g->node_names[current]);
        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adj_matrix[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                queue[rear++] = i;
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph g;
    initialize_graph(&g);
    char input[256];
    char node1[32], node2[32];
    int weight;
    int choice;
    
    while (1) {
        printf("\nGraph Operations:\n");
        printf("1. Add node\n");
        printf("2. Add edge\n");
        printf("3. Depth First Search\n");
        printf("4. Breadth First Search\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        if (sscanf(input, "%d", &choice) != 1) {
            printf("Invalid input\n");
            continue;
        }
        
        switch (choice) {
            case 1:
                printf("Enter node name: ");
                if (fgets(input, sizeof(input), stdin) == NULL) break;
                input[strcspn(input, "\n")] = '\0';
                if (add_node(&g, input) == -1) {
                    printf("Failed to add node\n");
                } else {
                    printf("Node added successfully\n");
                }
                break;
                
            case 2:
                printf("Enter source node: ");
                if (fgets(node1, sizeof(node1), stdin) == NULL) break;
                node1[strcspn(node1, "\