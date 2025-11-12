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

void initialize_graph(struct Graph* graph, int num_nodes) {
    if (num_nodes <= 0 || num_nodes > MAX_NODES) {
        num_nodes = MAX_NODES;
    }
    graph->num_nodes = num_nodes;
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            graph->adj_matrix[i][j] = 0;
        }
    }
}

void add_edge(struct Graph* graph, int src, int dest) {
    if (src < 0 || src >= graph->num_nodes || dest < 0 || dest >= graph->num_nodes) {
        return;
    }
    graph->adj_matrix[src][dest] = 1;
    graph->adj_matrix[dest][src] = 1;
}

struct Queue* create_queue() {
    struct Queue* queue = malloc(sizeof(struct Queue));
    if (queue == NULL) {
        exit(1);
    }
    queue->front = -1;
    queue->rear = -1;
    return queue;
}

int is_empty(struct Queue* queue) {
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
    if (is_empty(queue)) {
        return -1;
    }
    int item = queue->items[queue->front];
    queue->front++;
    if (queue->front > queue->rear) {
        queue->front = queue->rear = -1;
    }
    return item;
}

void bfs(struct Graph* graph, int start_vertex) {
    if (start_vertex < 0 || start_vertex >= graph->num_nodes) {
        return;
    }
    int visited[MAX_NODES] = {0};
    struct Queue* queue = create_queue();
    visited[start_vertex] = 1;
    enqueue(queue, start_vertex);
    printf("BFS traversal: ");
    while (!is_empty(queue)) {
        int current_vertex = dequeue(queue);
        printf("%d ", current_vertex);
        for (int i = 0; i < graph->num_nodes; i++) {
            if (graph->adj_matrix[current_vertex][i] == 1 && !visited[i]) {
                visited[i] = 1;
                enqueue(queue, i);
            }
        }
    }
    printf("\n");
    free(queue);
}

void dfs_util(struct Graph* graph, int vertex, int visited[]) {
    visited[vertex] = 1;
    printf("%d ", vertex);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (graph->adj_matrix[vertex][i] == 1 && !visited[i]) {
            dfs_util(graph, i, visited);
        }
    }
}

void dfs(struct Graph* graph, int start_vertex) {
    if (start_vertex < 0 || start_vertex >= graph->num_nodes) {
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_util(graph, start_vertex, visited);
    printf("\n");
}

int main() {
    struct Graph graph;
    int num_nodes, num_edges;
    printf("Enter number of nodes (max %d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (num_nodes <= 0 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    initialize_graph(&graph, num_nodes);
    printf("Enter number of edges: ");
    if (scanf("%d", &num_edges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (num_edges < 0 || num_edges > num_nodes * (num_nodes - 1) / 2) {
        printf("Invalid number of edges\n");
        return 1;
    }
    printf("Enter edges (source destination):\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest;
        if (scanf("%d %d", &src, &dest) != 2) {
            printf("Invalid input\n");
            return 1;
        }
        if (src < 0 || src >= num_nodes || dest < 0 || dest >= num_nodes) {
            printf("Invalid node indices\n");
            return 1;
        }
        add_edge(&graph, src, dest);
    }
    int start_vertex;
    printf("Enter starting vertex for traversals: ");
    if (scanf("%d", &start_vertex) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (start_vertex < 0 || start_vertex >= num_nodes) {
        printf("Invalid starting vertex\n");