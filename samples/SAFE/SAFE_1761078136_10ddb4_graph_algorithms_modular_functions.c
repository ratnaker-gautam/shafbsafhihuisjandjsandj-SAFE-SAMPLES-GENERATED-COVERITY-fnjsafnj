//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
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

void initialize_queue(struct Queue* queue) {
    queue->front = -1;
    queue->rear = -1;
}

int is_queue_empty(struct Queue* queue) {
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
    if (is_queue_empty(queue)) {
        return -1;
    }
    int item = queue->items[queue->front];
    queue->front++;
    if (queue->front > queue->rear) {
        queue->front = queue->rear = -1;
    }
    return item;
}

void bfs(struct Graph* graph, int start_node) {
    if (start_node < 0 || start_node >= graph->num_nodes) {
        return;
    }
    int visited[MAX_NODES] = {0};
    struct Queue queue;
    initialize_queue(&queue);
    visited[start_node] = 1;
    enqueue(&queue, start_node);
    printf("BFS traversal: ");
    while (!is_queue_empty(&queue)) {
        int current_node = dequeue(&queue);
        printf("%d ", current_node);
        for (int i = 0; i < graph->num_nodes; i++) {
            if (graph->adj_matrix[current_node][i] == 1 && !visited[i]) {
                visited[i] = 1;
                enqueue(&queue, i);
            }
        }
    }
    printf("\n");
}

void dfs_util(struct Graph* graph, int node, int visited[]) {
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (graph->adj_matrix[node][i] == 1 && !visited[i]) {
            dfs_util(graph, i, visited);
        }
    }
}

void dfs(struct Graph* graph, int start_node) {
    if (start_node < 0 || start_node >= graph->num_nodes) {
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_util(graph, start_node, visited);
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
            printf("Invalid node index\n");
            return 1;
        }
        add_edge(&graph, src, dest);
    }
    int start_node;
    printf("Enter starting node for traversals: ");
    if (scanf("%d", &start_node) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (start_node < 0 || start_node >= num_nodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    bfs(&graph, start_node);
    dfs(&graph, start_node);
    return 0;
}