//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int numNodes;
    int adjMatrix[MAX_NODES][MAX_NODES];
};

struct Queue {
    int items[MAX_NODES];
    int front;
    int rear;
};

void initializeGraph(struct Graph* graph, int numNodes) {
    if (numNodes <= 0 || numNodes > MAX_NODES) {
        numNodes = MAX_NODES;
    }
    graph->numNodes = numNodes;
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            graph->adjMatrix[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* graph, int src, int dest) {
    if (src < 0 || src >= graph->numNodes || dest < 0 || dest >= graph->numNodes) {
        return;
    }
    graph->adjMatrix[src][dest] = 1;
    graph->adjMatrix[dest][src] = 1;
}

struct Queue* createQueue() {
    struct Queue* queue = malloc(sizeof(struct Queue));
    if (queue != NULL) {
        queue->front = -1;
        queue->rear = -1;
    }
    return queue;
}

int isEmpty(struct Queue* queue) {
    if (queue == NULL) return 1;
    return queue->front == -1;
}

void enqueue(struct Queue* queue, int value) {
    if (queue == NULL) return;
    if (queue->rear == MAX_NODES - 1) return;
    if (queue->front == -1) {
        queue->front = 0;
    }
    queue->rear++;
    queue->items[queue->rear] = value;
}

int dequeue(struct Queue* queue) {
    if (queue == NULL || queue->front == -1) return -1;
    int item = queue->items[queue->front];
    queue->front++;
    if (queue->front > queue->rear) {
        queue->front = queue->rear = -1;
    }
    return item;
}

void bfs(struct Graph* graph, int startVertex) {
    if (graph == NULL || startVertex < 0 || startVertex >= graph->numNodes) return;
    
    int visited[MAX_NODES];
    for (int i = 0; i < graph->numNodes; i++) {
        visited[i] = 0;
    }
    
    struct Queue* queue = createQueue();
    if (queue == NULL) return;
    
    visited[startVertex] = 1;
    enqueue(queue, startVertex);
    
    printf("BFS traversal: ");
    
    while (!isEmpty(queue)) {
        int currentVertex = dequeue(queue);
        if (currentVertex == -1) break;
        
        printf("%d ", currentVertex);
        
        for (int i = 0; i < graph->numNodes; i++) {
            if (graph->adjMatrix[currentVertex][i] == 1 && !visited[i]) {
                visited[i] = 1;
                enqueue(queue, i);
            }
        }
    }
    printf("\n");
    free(queue);
}

void dfsUtil(struct Graph* graph, int vertex, int visited[]) {
    if (vertex < 0 || vertex >= graph->numNodes || visited[vertex]) return;
    
    visited[vertex] = 1;
    printf("%d ", vertex);
    
    for (int i = 0; i < graph->numNodes; i++) {
        if (graph->adjMatrix[vertex][i] == 1 && !visited[i]) {
            dfsUtil(graph, i, visited);
        }
    }
}

void dfs(struct Graph* graph, int startVertex) {
    if (graph == NULL || startVertex < 0 || startVertex >= graph->numNodes) return;
    
    int visited[MAX_NODES];
    for (int i = 0; i < graph->numNodes; i++) {
        visited[i] = 0;
    }
    
    printf("DFS traversal: ");
    dfsUtil(graph, startVertex, visited);
    printf("\n");
}

int main() {
    struct Graph graph;
    int numNodes, numEdges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &numNodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (numNodes <= 0 || numNodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    initializeGraph(&graph, numNodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &numEdges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (numEdges < 0 || numEdges > numNodes * (numNodes - 1) / 2) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (source destination):\n");
    for (int i = 0; i < numEdges; i++) {
        int src, dest;
        if (scanf("%d %d", &src, &dest) != 2) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (src < 0 || src >= numNodes || dest < 0 || dest >=