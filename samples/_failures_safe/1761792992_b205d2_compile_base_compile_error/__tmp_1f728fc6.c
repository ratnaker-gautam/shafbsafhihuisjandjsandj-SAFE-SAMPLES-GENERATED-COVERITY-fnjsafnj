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
                queue[rear++] = i;
                if (rear >= MAX_NODES) {
                    rear = 0;
                }
            }
        }
    }
}

int read_int(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min, max);
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    int choice;
    int node_id, from, to, weight;
    int visited[MAX_NODES] = {0};

    printf("Graph Algorithm Demo\n");
    printf("1. Add node\n");
    printf("2. Add edge\n");
    printf("3. Depth First Search\n");
    printf("4. Breadth First Search\n");
    printf("5. Exit\n");

    while (1) {
        choice = read_int("Enter choice: ", 1, 5);
        if (choice == -1) break;

        switch (choice) {
            case 1:
                node_id = read_int("Enter node ID (0-99): ", 0, 99);
                if (node_id == -1) break;
                if (add_node(&graph, node_id) == -1) {
                    printf("Failed to add node.\n");
                } else {
                    printf("Node added successfully.\n");
                }
                break;
            case 2:
                from = read_int("Enter source node: ", 0, 99);
                if (from == -1) break;
                to = read_int("Enter destination node: ", 0, 99);
                if (to == -1) break;
                weight = read_int("Enter edge weight: ", 0, 1000);
                if (weight == -1) break;
                if (add_edge(&graph, from, to, weight) == -1) {
                    printf("Failed to add edge.\n");
                } else {
                    printf("Edge added successfully.\n");
                }
                break;
            case 3:
                node_id = read_int("Enter start node for DFS: ", 0, 99);
                if (node_id == -1) break;
                memset(visited, 0, sizeof(visited));
                printf("DFS traversal: ");
                depth_first_search(&