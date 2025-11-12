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
    queue[rear++] = start;
    g->visited[start] = 1;
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        for (int i = 0; i < MAX_NODES; i++) {
            if (g->adjacency[current][i] && !g->visited[i]) {
                queue[rear++] = i;
                g->visited[i] = 1;
                if (rear >= MAX_NODES) {
                    rear = MAX_NODES - 1;
                }
            }
        }
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    int choice, u, v, start;
    printf("Graph Operations Menu:\n");
    printf("1. Add edge\n");
    printf("2. Perform DFS\n");
    printf("3. Perform BFS\n");
    printf("4. Exit\n");
    while (1) {
        printf("\nEnter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        if (choice == 1) {
            printf("Enter two nodes (0-%d): ", MAX_NODES - 1);
            if (scanf("%d %d", &u, &v) != 2) {
                printf("Invalid input\n");
                while (getchar() != '\n');
                continue;
            }
            if (u < 0 || u >= MAX_NODES || v < 0 || v >= MAX_NODES) {
                printf("Invalid node range\n");
                continue;
            }
            if (add_edge(&graph, u, v)) {
                printf("Edge added between %d and %d\n", u, v);
            } else {
                printf("Edge already exists or invalid\n");
            }
        } else if (choice == 2) {
            printf("Enter start node: ");
            if (scanf("%d", &start) != 1) {
                printf("Invalid input\n");
                while (getchar() != '\n');
                continue;
            }
            if (start < 0 || start >= MAX_NODES) {
                printf("Invalid node\n");
                continue;
            }
            for (int i = 0; i < MAX_NODES; i++) {
                graph.visited[i] = 0;
            }
            printf("DFS traversal: ");
            dfs(&graph, start);
            printf("\n");
        } else if (choice == 3) {
            printf("Enter start node: ");
            if (scanf("%d", &start) != 1) {
                printf("Invalid input\n");
                while (getchar() != '\n');
                continue;
            }
            if (start < 0 || start >= MAX_NODES) {
                printf("Invalid node\n");
                continue;
            }
            printf("BFS traversal: ");
            bfs(&graph, start);
            printf("\n");
        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }
    return 0;
}