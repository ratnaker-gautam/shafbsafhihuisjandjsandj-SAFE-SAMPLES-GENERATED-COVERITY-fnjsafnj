//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
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

void depth_first_search(struct Graph *g, int start, int visited[]) {
    if (start < 0 || start >= g->num_nodes) {
        return;
    }
    visited[start] = 1;
    printf("%d ", start);
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adjacency[start][i] == 1 && !visited[i]) {
            depth_first_search(g, i, visited);
        }
    }
}

void breadth_first_search(struct Graph *g, int start) {
    if (start < 0 || start >= g->num_nodes) {
        return;
    }
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adjacency[current][i] == 1 && !visited[i]) {
                visited[i] = 1;
                queue[rear++] = i;
                if (rear >= MAX_NODES) {
                    rear = 0;
                }
            }
        }
    }
}

int main() {
    struct Graph g;
    initialize_graph(&g);
    int choice, from, to, weight, start;
    char input[100];
    printf("Graph Operations Menu:\n");
    printf("1. Add node\n");
    printf("2. Add edge\n");
    printf("3. Depth First Search\n");
    printf("4. Breadth First Search\n");
    printf("5. Exit\n");
    while (1) {
        printf("\nEnter choice: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        if (sscanf(input, "%d", &choice) != 1) {
            printf("Invalid input\n");
            continue;
        }
        switch (choice) {
            case 1:
                if (add_node(&g) == -1) {
                    printf("Cannot add more nodes\n");
                } else {
                    printf("Node added. Total nodes: %d\n", g.num_nodes);
                }
                break;
            case 2:
                printf("Enter from, to, weight: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    break;
                }
                if (sscanf(input, "%d %d %d", &from, &to, &weight) != 3) {
                    printf("Invalid input\n");
                    break;
                }
                if (add_edge(&g, from, to, weight) == -1) {
                    printf("Invalid edge\n");
                } else {
                    printf("Edge added\n");
                }
                break;
            case 3:
                printf("Enter start node: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    break;
                }
                if (sscanf(input, "%d", &start) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                if (start < 0 || start >= g.num_nodes) {
                    printf("Invalid node\n");
                    break;
                }
                printf("DFS: ");
                int visited[MAX_NODES] = {0};
                depth_first_search(&g, start, visited);
                printf("\n");
                break;
            case 4:
                printf("Enter start node: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    break;
                }
                if (sscanf(input, "%d", &start) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                if (start < 0 || start >= g.num_nodes) {
                    printf("Invalid node\n");
                    break;
                }
                printf("BFS: ");
                breadth_first_search(&g, start);
                printf("\n");