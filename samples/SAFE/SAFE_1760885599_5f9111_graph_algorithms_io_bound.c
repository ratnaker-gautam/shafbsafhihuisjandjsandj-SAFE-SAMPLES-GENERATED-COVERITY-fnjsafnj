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

int add_node(struct Graph *g) {
    if (g->num_nodes >= MAX_NODES) {
        return -1;
    }
    return g->num_nodes++;
}

int add_edge(struct Graph *g, int from, int to) {
    if (from < 0 || from >= g->num_nodes || to < 0 || to >= g->num_nodes) {
        return -1;
    }
    if (g->adjacency[from][to] == 0) {
        g->adjacency[from][to] = 1;
        g->num_edges++;
        return 0;
    }
    return -1;
}

void dfs(struct Graph *g, int node) {
    if (node < 0 || node >= g->num_nodes || g->visited[node]) {
        return;
    }
    g->visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adjacency[node][i] && !g->visited[i]) {
            dfs(g, i);
        }
    }
}

void bfs(struct Graph *g, int start) {
    if (start < 0 || start >= g->num_nodes) {
        return;
    }
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    for (int i = 0; i < g->num_nodes; i++) {
        g->visited[i] = 0;
    }
    g->visited[start] = 1;
    queue[rear++] = start;
    printf("%d ", start);
    while (front < rear) {
        int current = queue[front++];
        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adjacency[current][i] && !g->visited[i]) {
                g->visited[i] = 1;
                printf("%d ", i);
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
    int choice, node1, node2, start;
    char input[100];
    printf("Graph Operations Menu:\n");
    printf("1. Add node\n");
    printf("2. Add edge\n");
    printf("3. DFS traversal\n");
    printf("4. BFS traversal\n");
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
                printf("Enter source and destination nodes: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    break;
                }
                if (sscanf(input, "%d %d", &node1, &node2) != 2) {
                    printf("Invalid input\n");
                    break;
                }
                if (add_edge(&g, node1, node2) == -1) {
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
                for (int i = 0; i < g.num_nodes; i++) {
                    g.visited[i] = 0;
                }
                printf("DFS: ");
                dfs(&g, start);
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
                printf("BFS: ");
                bfs(&g, start);
                printf("\n");
                break;
            case 5:
                return 0;
            default:
                printf("Invalid choice\n");
        }
    }
    return 0;
}