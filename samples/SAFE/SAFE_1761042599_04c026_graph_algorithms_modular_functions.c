//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
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
        if (current == -1) {
            break;
        }
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
        int minIndex = -1;
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                minIndex = v;
            }
        }
        if (minIndex == -1) {
            break;
        }
        visited[minIndex] = 1;
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && g->matrix[minIndex][v] != 0 && dist[minIndex] != INF) {
                long newDist = (long)dist[minIndex] + g->matrix[minIndex][v];
                if (newDist < dist[v] && newDist <= INT_MAX) {
                    dist[v] = (int)newDist;
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
    struct Graph g;
    int nodes = 6;
    initGraph(&g, nodes);
    addEdge(&g, 0, 1, 4);
    addEdge(&g, 0, 2, 2);
    addEdge(&g, 1, 2, 1);
    addEdge(&g, 1, 3, 5);
    addEdge(&g, 2, 3, 8);
    addEdge(&g, 2, 4, 10);
    addEdge(&g, 3, 4, 2);
    addEdge(&g, 3, 5, 6);
    addEdge(&g, 4, 5, 3);
    bfs(&g, 0);
    dijkstra(&g, 0);
    return 0;
}