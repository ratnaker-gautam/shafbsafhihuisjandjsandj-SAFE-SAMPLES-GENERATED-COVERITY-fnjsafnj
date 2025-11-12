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

int isValidNode(struct Graph* graph, int node) {
    return node >= 0 && node < graph->numNodes;
}

void addEdge(struct Graph* graph, int src, int dest) {
    if (isValidNode(graph, src) && isValidNode(graph, dest)) {
        graph->adjMatrix[src][dest] = 1;
        graph->adjMatrix[dest][src] = 1;
    }
}

void initializeQueue(struct Queue* q) {
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

void bfs(struct Graph* graph, int startNode) {
    if (!isValidNode(graph, startNode)) {
        return;
    }
    int visited[MAX_NODES] = {0};
    struct Queue q;
    initializeQueue(&q);
    visited[startNode] = 1;
    enqueue(&q, startNode);
    printf("BFS traversal: ");
    while (!isEmpty(&q)) {
        int currentNode = dequeue(&q);
        printf("%d ", currentNode);
        for (int i = 0; i < graph->numNodes; i++) {
            if (graph->adjMatrix[currentNode][i] == 1 && !visited[i]) {
                visited[i] = 1;
                enqueue(&q, i);
            }
        }
    }
    printf("\n");
}

void dfsUtil(struct Graph* graph, int node, int visited[]) {
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < graph->numNodes; i++) {
        if (graph->adjMatrix[node][i] == 1 && !visited[i]) {
            dfsUtil(graph, i, visited);
        }
    }
}

void dfs(struct Graph* graph, int startNode) {
    if (!isValidNode(graph, startNode)) {
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfsUtil(graph, startNode, visited);
    printf("\n");
}

int main() {
    struct Graph graph;
    int numNodes, numEdges;
    printf("Enter number of nodes (max %d): ", MAX_NODES);
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
        if (src < 0 || src >= numNodes || dest < 0 || dest >= numNodes) {
            printf("Invalid node in edge\n");
            return 1;
        }
        addEdge(&graph, src, dest);
    }
    int startNode;
    printf("Enter starting node for traversals: ");
    if (scanf("%d", &startNode) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (startNode < 0 || startNode >= numNodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    bfs(&graph, startNode);
    dfs(&graph, startNode);
    return 0;
}