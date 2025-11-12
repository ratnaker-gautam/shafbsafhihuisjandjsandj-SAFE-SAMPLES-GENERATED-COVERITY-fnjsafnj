//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int nodes[MAX_NODES];
    int edges[MAX_NODES][MAX_NODES];
    int node_count;
    int edge_count;
};

void initialize_graph(struct Graph *g) {
    g->node_count = 0;
    g->edge_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        g->nodes[i] = 0;
        for (int j = 0; j < MAX_NODES; j++) {
            g->edges[i][j] = 0;
        }
    }
}

int add_node(struct Graph *g, int node_id) {
    if (g->node_count >= MAX_NODES) {
        return -1;
    }
    if (node_id < 0 || node_id >= MAX_NODES) {
        return -1;
    }
    g->nodes[g->node_count] = node_id;
    g->node_count++;
    return 0;
}

int add_edge(struct Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= MAX_NODES || to < 0 || to >= MAX_NODES) {
        return -1;
    }
    if (g->edge_count >= MAX_EDGES) {
        return -1;
    }
    if (weight < 0) {
        return -1;
    }
    g->edges[from][to] = weight;
    g->edges[to][from] = weight;
    g->edge_count++;
    return 0;
}

void depth_first_search(struct Graph *g, int start, int visited[]) {
    if (start < 0 || start >= MAX_NODES) {
        return;
    }
    visited[start] = 1;
    printf("%d ", start);
    for (int i = 0; i < MAX_NODES; i++) {
        if (g->edges[start][i] != 0 && !visited[i]) {
            depth_first_search(g, i, visited);
        }
    }
}

void breadth_first_search(struct Graph *g, int start) {
    if (start < 0 || start >= MAX_NODES) {
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
        for (int i = 0; i < MAX_NODES; i++) {
            if (g->edges[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
}

int main() {
    struct Graph g;
    initialize_graph(&g);
    int choice, node1, node2, weight;
    char input[100];
    while (1) {
        printf("\nGraph Operations:\n");
        printf("1. Add Node\n");
        printf("2. Add Edge\n");
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
                printf("Enter node ID (0-%d): ", MAX_NODES - 1);
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    break;
                }
                if (sscanf(input, "%d", &node1) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                if (add_node(&g, node1) == 0) {
                    printf("Node added\n");
                } else {
                    printf("Failed to add node\n");
                }
                break;
            case 2:
                printf("Enter from node: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    break;
                }
                if (sscanf(input, "%d", &node1) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                printf("Enter to node: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    break;
                }
                if (sscanf(input, "%d", &node2) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                printf("Enter weight: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    break;
                }
                if (sscanf(input, "%d", &weight) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                if (add_edge(&g, node1, node2, weight) == 0) {
                    printf("Edge added\n");
                } else {
                    printf("Failed to add edge\n");
                }
                break;
            case 3:
                {
                    printf("Enter start node: ");
                    if (fgets(input, sizeof(input), stdin) == NULL) {