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
    if (weight < 0) {
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
            if (g->adj_matrix[i][j] > 0) {
                printf("  %s -> %s (weight: %d)\n", 
                       g->node_names[i], g->node_names[j], g->adj_matrix[i][j]);
            }
        }
    }
}

void bfs_traversal(struct Graph *g, const char *start_name) {
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
            if (g->adj_matrix[current][i] > 0 && !visited[i]) {
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
    add_edge(&g, "E", "A", 2);
    
    print_graph(&g);
    printf("\n");
    
    bfs_traversal(&g, "A");
    bfs_traversal(&g, "D");
    
    printf("\nAdding invalid edge...\n");
    if (add_edge(&g, "X", "Y", 1) == -1) {
        printf("Failed to add edge between non-existent nodes.\n");
    }
    
    printf("\nAdding duplicate node...\n");
    if (add_node(&g, "A") == -1) {
        printf("Failed to add duplicate node.\n");
    }
    
    printf("\nAdding too long node name...\n");
    char long_name[40];
    memset(long_name, 'X', 39);
    long