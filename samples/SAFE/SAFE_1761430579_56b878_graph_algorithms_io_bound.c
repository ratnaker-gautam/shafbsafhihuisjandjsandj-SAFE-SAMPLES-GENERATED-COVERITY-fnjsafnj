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
    int visited[MAX_NODES];
};

void initialize_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g->adjacency[i][j] = 0;
        }
        g->visited[i] = 0;
    }
}

int add_edge(struct Graph *g, int u, int v) {
    if (u < 0 || u >= MAX_NODES || v < 0 || v >= MAX_NODES) {
        return 0;
    }
    if (g->adjacency[u][v] == 0) {
        g->adjacency[u][v] = 1;
        g->adjacency[v][u] = 1;
        g->num_edges++;
        return 1;
    }
    return 0;
}

void dfs(struct Graph *g, int node) {
    if (node < 0 || node >= MAX_NODES || g->visited[node]) {
        return;
    }
    g->visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < MAX_NODES; i++) {
        if (g->adjacency[node][i] && !g->visited[i]) {
            dfs(g, i);
        }
    }
}

void bfs(struct Graph *g, int start) {
    if (start < 0 || start >= MAX_NODES) {
        return;
    }
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        g->visited[i] = 0;
    }
    g->visited[start] = 1;
    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        for (int i = 0; i < MAX_NODES; i++) {
            if (g->adjacency[current][i] && !g->visited[i]) {
                g->visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    int choice, u, v, start;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &graph.num_nodes) != 1 || graph.num_nodes < 1 || graph.num_nodes > MAX_NODES) {
        printf("Invalid number of nodes.\n");
        return 1;
    }
    printf("Enter edges (u v), -1 -1 to stop:\n");
    while (1) {
        if (scanf("%d %d", &u, &v) != 2) {
            printf("Invalid input.\n");
            return 1;
        }
        if (u == -1 && v == -1) {
            break;
        }
        if (u < 0 || u >= graph.num_nodes || v < 0 || v >= graph.num_nodes) {
            printf("Invalid edge.\n");
            continue;
        }
        if (!add_edge(&graph, u, v)) {
            printf("Failed to add edge.\n");
        }
    }
    printf("Choose traversal: 1 for DFS, 2 for BFS: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice.\n");
        return 1;
    }
    printf("Enter start node: ");
    if (scanf("%d", &start) != 1 || start < 0 || start >= graph.num_nodes) {
        printf("Invalid start node.\n");
        return 1;
    }
    if (choice == 1) {
        printf("DFS traversal: ");
        dfs(&graph, start);
        printf("\n");
    } else if (choice == 2) {
        printf("BFS traversal: ");
        bfs(&graph, start);
        printf("\n");
    } else {
        printf("Invalid choice.\n");
        return 1;
    }
    return 0;
}