//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int nodes[MAX_NODES];
    int edges[MAX_EDGES][2];
    int node_count;
    int edge_count;
};

void init_graph(struct Graph *g) {
    g->node_count = 0;
    g->edge_count = 0;
    memset(g->nodes, 0, sizeof(g->nodes));
    memset(g->edges, 0, sizeof(g->edges));
}

int add_node(struct Graph *g, int node) {
    if (g->node_count >= MAX_NODES) return 0;
    if (node < 0 || node >= MAX_NODES) return 0;
    
    int *ptr = g->nodes;
    for (int i = 0; i < g->node_count; i++) {
        if (*(ptr + i) == node) return 0;
    }
    
    *(g->nodes + g->node_count) = node;
    g->node_count++;
    return 1;
}

int add_edge(struct Graph *g, int from, int to) {
    if (g->edge_count >= MAX_EDGES) return 0;
    if (from < 0 || from >= MAX_NODES) return 0;
    if (to < 0 || to >= MAX_NODES) return 0;
    
    int found_from = 0, found_to = 0;
    int *node_ptr = g->nodes;
    for (int i = 0; i < g->node_count; i++) {
        if (*(node_ptr + i) == from) found_from = 1;
        if (*(node_ptr + i) == to) found_to = 1;
    }
    if (!found_from || !found_to) return 0;
    
    int (*edge_ptr)[2] = g->edges;
    for (int i = 0; i < g->edge_count; i++) {
        if (*(*(edge_ptr + i) + 0) == from && *(*(edge_ptr + i) + 1) == to) {
            return 0;
        }
    }
    
    *(*(g->edges + g->edge_count) + 0) = from;
    *(*(g->edges + g->edge_count) + 1) = to;
    g->edge_count++;
    return 1;
}

void dfs(struct Graph *g, int node, int *visited) {
    if (node < 0 || node >= MAX_NODES) return;
    if (*(visited + node)) return;
    
    *(visited + node) = 1;
    printf("%d ", node);
    
    int (*edge_ptr)[2] = g->edges;
    for (int i = 0; i < g->edge_count; i++) {
        if (*(*(edge_ptr + i) + 0) == node) {
            int next_node = *(*(edge_ptr + i) + 1);
            if (!*(visited + next_node)) {
                dfs(g, next_node, visited);
            }
        }
    }
}

void bfs(struct Graph *g, int start) {
    if (start < 0 || start >= MAX_NODES) return;
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    *(visited + start) = 1;
    *(queue + rear) = start;
    rear++;
    
    while (front < rear) {
        int current = *(queue + front);
        front++;
        printf("%d ", current);
        
        int (*edge_ptr)[2] = g->edges;
        for (int i = 0; i < g->edge_count; i++) {
            if (*(*(edge_ptr + i) + 0) == current) {
                int neighbor = *(*(edge_ptr + i) + 1);
                if (!*(visited + neighbor)) {
                    *(visited + neighbor) = 1;
                    *(queue + rear) = neighbor;
                    rear++;
                    if (rear >= MAX_NODES) rear = 0;
                }
            }
        }
    }
}

int main() {
    struct Graph g;
    init_graph(&g);
    
    printf("Graph Operations:\n");
    printf("1. Add node\n");
    printf("2. Add edge\n");
    printf("3. DFS traversal\n");
    printf("4. BFS traversal\n");
    printf("5. Exit\n");
    
    int choice;
    while (1) {
        printf("\nEnter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (choice == 1) {
            int node;
            printf("Enter node: ");
            if (scanf("%d", &node) != 1) {
                printf("Invalid input\n");
                while (getchar() != '\n');
                continue;
            }
            if (add_node(&g, node)) {
                printf("Node added\n");
            } else {
                printf("Failed to add node\n");
            }
        } else if (choice == 2) {
            int from, to;
            printf("Enter from and to nodes: ");
            if (scanf("%d %d", &from, &to) != 2) {
                printf("Invalid input\n");
                while (getchar() != '\n