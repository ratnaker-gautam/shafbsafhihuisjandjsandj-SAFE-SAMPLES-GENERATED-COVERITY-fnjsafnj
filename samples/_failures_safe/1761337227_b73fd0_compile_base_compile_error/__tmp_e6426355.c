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

void depth_first_search(struct Graph *g, int start_idx, int *visited) {
    if (visited[start_idx]) {
        return;
    }
    visited[start_idx] = 1;
    printf("%s ", g->node_names[start_idx]);
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adj_matrix[start_idx][i] > 0 && !visited[i]) {
            depth_first_search(g, i, visited);
        }
    }
}

void breadth_first_search(struct Graph *g, int start_idx) {
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start_idx] = 1;
    queue[rear++] = start_idx;
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
}

int main() {
    struct Graph g;
    initialize_graph(&g);
    char input[64];
    char node1[32], node2[32];
    int weight;
    int choice;
    
    while (1) {
        printf("\nGraph Operations:\n");
        printf("1. Add node\n");
        printf("2. Add edge\n");
        printf("3. DFS traversal\n");
        printf("4. BFS traversal\n");
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
                node1[strcspn(node1, "\n")] = '\0';
                printf("Enter destination node: ");
                if (fgets(node2, sizeof(node2), stdin) == NULL) break;
                node2[strcspn(node2, "\n")] = '\0';
                printf("Enter weight: ");
                if (fgets(input, sizeof(input), stdin) == NULL) break;
                if (sscanf(input, "%d", &weight) != 1) {
                    printf("Invalid weight\n");
                    break;
                }
                if (add_edge(&g, node1, node2, weight) == -1) {
                    printf("Failed to add edge\n");
                } else {
                    printf("Edge added successfully\n");
                }
                break;
                
            case 3:
                if (g.num_nodes == 0) {
                    printf("Graph is empty\n");
                    break;