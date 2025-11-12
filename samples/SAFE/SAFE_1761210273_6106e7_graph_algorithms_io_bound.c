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
    int edge_weights[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g->adjacency[i][j] = 0;
            g->edge_weights[i][j] = 0;
        }
    }
}

int add_node(struct Graph *g) {
    if (g->num_nodes >= MAX_NODES) {
        return -1;
    }
    return g->num_nodes++;
}

int add_edge(struct Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->num_nodes || to < 0 || to >= g->num_nodes) {
        return -1;
    }
    if (g->adjacency[from][to] == 1) {
        return -1;
    }
    g->adjacency[from][to] = 1;
    g->edge_weights[from][to] = weight;
    g->num_edges++;
    return 0;
}

void dfs_util(struct Graph *g, int node, int visited[]) {
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adjacency[node][i] && !visited[i]) {
            dfs_util(g, i, visited);
        }
    }
}

void depth_first_search(struct Graph *g, int start) {
    if (start < 0 || start >= g->num_nodes) {
        printf("Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
}

void breadth_first_search(struct Graph *g, int start) {
    if (start < 0 || start >= g->num_nodes) {
        printf("Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    printf("BFS traversal: ");
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adjacency[current][i] && !visited[i]) {
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
    int choice, node1, node2, weight, start_node;
    int node_count = 0;
    
    printf("Graph Operations Menu:\n");
    printf("1. Add node\n");
    printf("2. Add edge\n");
    printf("3. Perform DFS\n");
    printf("4. Perform BFS\n");
    printf("5. Exit\n");
    
    while (1) {
        printf("\nEnter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1:
                if (add_node(&g) == -1) {
                    printf("Cannot add more nodes\n");
                } else {
                    printf("Added node %d\n", g.num_nodes - 1);
                }
                break;
                
            case 2:
                printf("Enter source, destination, weight: ");
                if (scanf("%d %d %d", &node1, &node2, &weight) != 3) {
                    printf("Invalid input\n");
                    while (getchar() != '\n');
                    break;
                }
                if (add_edge(&g, node1, node2, weight) == -1) {
                    printf("Failed to add edge\n");
                } else {
                    printf("Edge added successfully\n");
                }
                break;
                
            case 3:
                printf("Enter start node for DFS: ");
                if (scanf("%d", &start_node) != 1) {
                    printf("Invalid input\n");
                    while (getchar() != '\n');
                    break;
                }
                depth_first_search(&g, start_node);
                break;
                
            case 4:
                printf("Enter start node for BFS: ");
                if (scanf("%d", &start_node) != 1) {
                    printf("Invalid input\n");
                    while (getchar() != '\n');
                    break;
                }
                breadth_first_search(&g, start_node);
                break;
                
            case 5:
                printf("Exiting program\n");
                return 0;
                
            default:
                printf("Invalid choice\n");
                break;
        }
    }
    
    return 0;
}