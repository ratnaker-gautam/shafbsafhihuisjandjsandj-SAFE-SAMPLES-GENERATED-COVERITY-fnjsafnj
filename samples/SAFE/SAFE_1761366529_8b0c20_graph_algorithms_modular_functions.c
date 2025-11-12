//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct Graph {
    int nodes;
    int matrix[MAX_NODES][MAX_NODES];
};

struct Queue {
    int items[MAX_NODES];
    int front;
    int rear;
};

void initGraph(struct Graph* g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        nodes = MAX_NODES;
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->matrix[i][j] = 0;
        }
    }
}

int isValidNode(struct Graph* g, int node) {
    return node >= 0 && node < g->nodes;
}

void addEdge(struct Graph* g, int src, int dest, int weight) {
    if (!isValidNode(g, src) || !isValidNode(g, dest)) {
        return;
    }
    if (weight < 0) {
        weight = 0;
    }
    g->matrix[src][dest] = weight;
    g->matrix[dest][src] = weight;
}

void initQueue(struct Queue* q) {
    q->front = -1;
    q->rear = -1;
}

int isEmpty(struct Queue* q) {
    return q->front == -1;
}

void enqueue(struct Queue* q, int value) {
    if (q->rear == MAX_NODES - 1) {
        return;
    }
    if (q->front == -1) {
        q->front = 0;
    }
    q->rear++;
    q->items[q->rear] = value;
}

int dequeue(struct Queue* q) {
    if (isEmpty(q)) {
        return -1;
    }
    int item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
        q->front = q->rear = -1;
    }
    return item;
}

void bfs(struct Graph* g, int start) {
    if (!isValidNode(g, start)) {
        return;
    }
    int visited[MAX_NODES] = {0};
    struct Queue q;
    initQueue(&q);
    visited[start] = 1;
    enqueue(&q, start);
    printf("BFS traversal: ");
    while (!isEmpty(&q)) {
        int current = dequeue(&q);
        printf("%d ", current);
        for (int i = 0; i < g->nodes; i++) {
            if (g->matrix[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                enqueue(&q, i);
            }
        }
    }
    printf("\n");
}

void dijkstra(struct Graph* g, int start) {
    if (!isValidNode(g, start)) {
        return;
    }
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INF;
    }
    dist[start] = 0;
    for (int count = 0; count < g->nodes - 1; count++) {
        int min = INF;
        int min_index = -1;
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                min_index = v;
            }
        }
        if (min_index == -1) {
            break;
        }
        visited[min_index] = 1;
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && g->matrix[min_index][v] != 0 && dist[min_index] != INF) {
                long new_dist = (long)dist[min_index] + g->matrix[min_index][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
    printf("Dijkstra distances from node %d:\n", start);
    for (int i = 0; i < g->nodes; i++) {
        if (dist[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph graph;
    int nodes = 6;
    initGraph(&graph, nodes);
    addEdge(&graph, 0, 1, 4);
    addEdge(&graph, 0, 2, 3);
    addEdge(&graph, 1, 2, 1);
    addEdge(&graph, 1, 3, 2);
    addEdge(&graph, 2, 3, 4);
    addEdge(&graph, 3, 4, 2);
    addEdge(&graph, 4, 5, 6);
    bfs(&graph, 0);
    dijkstra(&graph, 0);
    return 0;
}