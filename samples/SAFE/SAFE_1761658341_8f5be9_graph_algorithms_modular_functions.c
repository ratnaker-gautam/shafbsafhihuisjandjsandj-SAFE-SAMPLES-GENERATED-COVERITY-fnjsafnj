//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int nodes;
    int adj[MAX_NODES][MAX_NODES];
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
            graph->adj[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* graph, int src, int dest) {
    if (src < 0 || src >= graph->nodes || dest < 0 || dest >= graph->nodes) {
        return;
    }
    graph->adj[src][dest] = 1;
    graph->adj[dest][src] = 1;
}

void initializeQueue(struct Queue* queue) {
    queue->front = -1;
    queue->rear = -1;
}

int isEmpty(struct Queue* queue) {
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
    if (isEmpty(queue)) {
        return -1;
    }
    int item = queue->items[queue->front];
    queue->front++;
    if (queue->front > queue->rear) {
        queue->front = queue->rear = -1;
    }
    return item;
}

void bfs(struct Graph* graph, int start) {
    if (start < 0 || start >= graph->nodes) {
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < graph->nodes; i++) {
        visited[i] = 0;
    }
    struct Queue queue;
    initializeQueue(&queue);
    visited[start] = 1;
    enqueue(&queue, start);
    printf("BFS traversal: ");
    while (!isEmpty(&queue)) {
        int current = dequeue(&queue);
        printf("%d ", current);
        for (int i = 0; i < graph->nodes; i++) {
            if (graph->adj[current][i] == 1 && !visited[i]) {
                visited[i] = 1;
                enqueue(&queue, i);
            }
        }
    }
    printf("\n");
}

void dfsUtil(struct Graph* graph, int vertex, int visited[]) {
    visited[vertex] = 1;
    printf("%d ", vertex);
    for (int i = 0; i < graph->nodes; i++) {
        if (graph->adj[vertex][i] == 1 && !visited[i]) {
            dfsUtil(graph, i, visited);
        }
    }
}

void dfs(struct Graph* graph, int start) {
    if (start < 0 || start >= graph->nodes) {
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < graph->nodes; i++) {
        visited[i] = 0;
    }
    printf("DFS traversal: ");
    dfsUtil(graph, start, visited);
    printf("\n");
}

int main() {
    struct Graph graph;
    int nodes, edges;
    printf("Enter number of nodes (max %d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || nodes <= 0 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    initializeGraph(&graph, nodes);
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        printf("Invalid number of edges\n");
        return 1;
    }
    printf("Enter edges (source destination):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest;
        if (scanf("%d %d", &src, &dest) != 2) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (src < 0 || src >= nodes || dest < 0 || dest >= nodes) {
            printf("Invalid node indices\n");
            return 1;
        }
        addEdge(&graph, src, dest);
    }
    int start;
    printf("Enter starting node for traversals: ");
    if (scanf("%d", &start) != 1 || start < 0 || start >= nodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    bfs(&graph, start);
    dfs(&graph, start);
    return 0;
}