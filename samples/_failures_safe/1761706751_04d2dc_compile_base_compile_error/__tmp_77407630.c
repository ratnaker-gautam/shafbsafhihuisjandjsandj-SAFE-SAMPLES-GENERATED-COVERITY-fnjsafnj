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
    if (weight <= 0) {
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

void breadth_first_search(struct Graph *g, int start_index) {
    if (start_index < 0 || start_index >= g->num_nodes) {
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start_index] = 1;
    queue[rear++] = start_index;
    
    printf("BFS traversal: ");
    
    while (front < rear) {
        int current = queue[front++];
        printf("%s ", g->node_names[current]);
        
        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adjacency[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
    printf("\n");
}

void depth_first_search_util(struct Graph *g, int node, int visited[]) {
    visited[node] = 1;
    printf("%s ", g->node_names[node]);
    
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adjacency[node][i] != 0 && !visited[i]) {
            depth_first_search_util(g, i, visited);
        }
    }
}

void depth_first_search(struct Graph *g, int start_index) {
    if (start_index < 0 || start_index >= g->num_nodes) {
        return;
    }
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    depth_first_search_util(g, start_index, visited);
    printf("\n");
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Graph Operations Menu:\n");
    printf("1. Add node\n");
    printf("2. Add edge\n");
    printf("3. BFS traversal\n");
    printf("4. DFS traversal\n");
    printf("5. Exit\n");
    
    char input[64];
    int choice;
    
    while (1) {
        printf("\nEnter choice (1-5): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (sscanf(input, "%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        switch (choice) {
            case 1: {
                printf("Enter node name: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    break;
                }
                input[strcspn(input, "\n")] = '\0';
                if (add_node(&graph, input) == -1) {
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
                
                printf("Enter destination node: ");
                if (fgets(to,