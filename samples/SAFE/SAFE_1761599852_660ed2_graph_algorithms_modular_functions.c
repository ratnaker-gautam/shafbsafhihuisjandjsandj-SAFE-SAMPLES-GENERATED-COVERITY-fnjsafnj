//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int num_nodes;
    int adj_matrix[MAX_NODES][MAX_NODES];
};

struct Queue {
    int items[MAX_NODES];
    int front;
    int rear;
};

void initialize_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        nodes = MAX_NODES;
    }
    g->num_nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj_matrix[i][j] = 0;
        }
    }
}

int add_edge(struct Graph *g, int src, int dest) {
    if (src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes) {
        return 0;
    }
    g->adj_matrix[src][dest] = 1;
    g->adj_matrix[dest][src] = 1;
    return 1;
}

void initialize_queue(struct Queue *q) {
    q->front = -1;
    q->rear = -1;
}

int is_queue_empty(struct Queue *q) {
    return q->front == -1;
}

void enqueue(struct Queue *q, int value) {
    if (q->rear == MAX_NODES - 1) {
        return;
    }
    if (q->front == -1) {
        q->front = 0;
    }
    q->rear++;
    q->items[q->rear] = value;
}

int dequeue(struct Queue *q) {
    if (is_queue_empty(q)) {
        return -1;
    }
    int item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
        q->front = q->rear = -1;
    }
    return item;
}

void bfs(struct Graph *g, int start) {
    if (start < 0 || start >= g->num_nodes) {
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        visited[i] = 0;
    }
    struct Queue q;
    initialize_queue(&q);
    visited[start] = 1;
    enqueue(&q, start);
    printf("BFS traversal: ");
    while (!is_queue_empty(&q)) {
        int current = dequeue(&q);
        printf("%d ", current);
        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adj_matrix[current][i] == 1 && !visited[i]) {
                visited[i] = 1;
                enqueue(&q, i);
            }
        }
    }
    printf("\n");
}

void dfs_util(struct Graph *g, int node, int visited[]) {
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adj_matrix[node][i] == 1 && !visited[i]) {
            dfs_util(g, i, visited);
        }
    }
}

void dfs(struct Graph *g, int start) {
    if (start < 0 || start >= g->num_nodes) {
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        visited[i] = 0;
    }
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
}

int main() {
    struct Graph g;
    int num_nodes = 6;
    initialize_graph(&g, num_nodes);
    add_edge(&g, 0, 1);
    add_edge(&g, 0, 2);
    add_edge(&g, 1, 3);
    add_edge(&g, 1, 4);
    add_edge(&g, 2, 4);
    add_edge(&g, 3, 5);
    add_edge(&g, 4, 5);
    bfs(&g, 0);
    dfs(&g, 0);
    return 0;
}