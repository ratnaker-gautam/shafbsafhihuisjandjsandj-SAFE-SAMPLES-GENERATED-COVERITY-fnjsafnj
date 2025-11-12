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

void initializeGraph(struct Graph* graph, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        nodes = MAX_NODES;
    }
    graph->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            graph->matrix[i][j] = 0;
        }
    }
}

int isValidNode(struct Graph* graph, int node) {
    return node >= 0 && node < graph->nodes;
}

void addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (!isValidNode(graph, src) || !isValidNode(graph, dest)) {
        return;
    }
    if (weight < 0) {
        weight = 0;
    }
    graph->matrix[src][dest] = weight;
    graph->matrix[dest][src] = weight;
}

void initializeQueue(struct Queue* queue) {
    queue->front = -1;
    queue->rear = -1;
}

int isQueueEmpty(struct Queue* queue) {
    return queue->front == -1;
}

void enqueue(struct Queue* queue, int value) {
    if (queue->rear == MAX_NODES - 1) {
        return;
    }
    if (queue->front == -1) {
        queue->front = 0;
    }
    queue->rear++;
    queue->items[queue->rear] = value;
}

int dequeue(struct Queue* queue) {
    if (isQueueEmpty(queue)) {
        return -1;
    }
    int item = queue->items[queue->front];
    queue->front++;
    if (queue->front > queue->rear) {
        initializeQueue(queue);
    }
    return item;
}

void bfs(struct Graph* graph, int start) {
    if (!isValidNode(graph, start)) {
        return;
    }
    int visited[MAX_NODES] = {0};
    struct Queue queue;
    initializeQueue(&queue);
    visited[start] = 1;
    enqueue(&queue, start);
    printf("BFS traversal: ");
    while (!isQueueEmpty(&queue)) {
        int current = dequeue(&queue);
        printf("%d ", current);
        for (int i = 0; i < graph->nodes; i++) {
            if (graph->matrix[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                enqueue(&queue, i);
            }
        }
    }
    printf("\n");
}

void dijkstra(struct Graph* graph, int start) {
    if (!isValidNode(graph, start)) {
        return;
    }
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < graph->nodes; i++) {
        dist[i] = INF;
    }
    dist[start] = 0;
    for (int count = 0; count < graph->nodes - 1; count++) {
        int min = INF;
        int minIndex = -1;
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                minIndex = v;
            }
        }
        if (minIndex == -1) {
            break;
        }
        visited[minIndex] = 1;
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && graph->matrix[minIndex][v] != 0 && dist[minIndex] != INF) {
                long newDist = (long)dist[minIndex] + graph->matrix[minIndex][v];
                if (newDist < dist[v] && newDist <= INT_MAX) {
                    dist[v] = (int)newDist;
                }
            }
        }
    }
    printf("Dijkstra distances from node %d:\n", start);
    for (int i = 0; i < graph->nodes; i++) {
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
    initializeGraph(&graph, nodes);
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