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
    if (weight < 0 || weight > 1000) {
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

void bfs(struct Graph *g, const char *start_name) {
    int start_idx = find_node_index(g, start_name);
    if (start_idx == -1) {
        printf("Start node not found.\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start_idx] = 1;
    queue[rear++] = start_idx;
    
    printf("BFS traversal from %s: ", start_name);
    
    while (front < rear) {
        int current = queue[front++];
        printf("%s ", g->node_names[current]);
        
        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adj_matrix[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph g;
    initialize_graph(&g);
    
    printf("Graph Construction:\n");
    printf("Enter nodes (type 'done' to finish):\n");
    
    char buffer[64];
    while (1) {
        printf("Enter node name: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strcmp(buffer, "done") == 0) break;
        if (add_node(&g, buffer) == -1) {
            printf("Failed to add node (duplicate or limit reached).\n");
        }
    }
    
    printf("\nEnter edges (format: from to weight, type 'done' to finish):\n");
    while (1) {
        printf("Enter edge: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strcmp(buffer, "done") == 0) break;
        
        char from[32], to[32];
        int weight;
        if (sscanf(buffer, "%31s %31s %d", from, to, &weight) == 3) {
            if (add_edge(&g, from, to, weight) == -1) {
                printf("Failed