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

void dijkstra(struct Graph *g, int start, int distances[]) {
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->num_nodes; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    for (int count = 0; count < g->num_nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_index = -1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!visited[v] && distances[v] <= min_dist) {
                min_dist = distances[v];
                min_index = v;
            }
        }
        
        if (min_index == -1) {
            break;
        }
        
        visited[min_index] = 1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!visited[v] && g->adj_matrix[min_index][v] != 0) {
                int new_dist = distances[min_index] + g->adj_matrix[min_index][v];
                if (new_dist < distances[v] && new_dist > 0) {
                    distances[v] = new_dist;
                }
            }
        }
    }
}

void print_shortest_paths(struct Graph *g, int start_node) {
    int distances[MAX_NODES];
    dijkstra(g, start_node, distances);
    
    printf("Shortest paths from node '%s':\n", g->node_names[start_node]);
    for (int i = 0; i < g->num_nodes; i++) {
        if (i != start_node) {
            if (distances[i] == INT_MAX) {
                printf("  to '%s': No path\n", g->node_names[i]);
            } else {
                printf("  to '%s': %d\n", g->node_names[i], distances[i]);
            }
        }
    }
}

int main() {
    struct Graph g;
    initialize_graph(&g);
    
    printf("Graph Shortest Path Calculator\n");
    printf("Enter nodes and edges to build a graph.\n");
    
    char buffer[256];
    int choice;
    
    while (1) {
        printf("\n1. Add node\n2. Add edge\n3. Calculate shortest paths\n4. Exit\nChoice: ");
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (sscanf(buffer, "%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        switch (choice) {
            case 1: {
                printf("Enter node name: ");
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    break;
                }
                buffer[strcspn(buffer, "\n")] = '\0';
                if (add_node(&g, buffer) == -1) {
                    printf("Failed to add node. Name might be too long, duplicate, or graph full.\n");
                } else {
                    printf("Node added successfully.\n");
                }
                break;
            }
            case 2: {
                char from[32], to[32];
                int weight;
                printf