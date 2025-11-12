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

int validate_graph_size(int nodes) {
    return nodes > 0 && nodes <= MAX_NODES;
}

void initialize_graph(struct Graph* graph, int nodes) {
    if (!graph || !validate_graph_size(nodes)) return;
    
    graph->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            graph->matrix[i][j] = 0;
        }
    }
}

int validate_node(struct Graph* graph, int node) {
    return graph && node >= 0 && node < graph->nodes;
}

void add_edge(struct Graph* graph, int src, int dest, int weight) {
    if (!graph || !validate_node(graph, src) || !validate_node(graph, dest)) return;
    if (weight < 0) return;
    
    graph->matrix[src][dest] = weight;
    graph->matrix[dest][src] = weight;
}

struct Queue* create_queue() {
    struct Queue* queue = malloc(sizeof(struct Queue));
    if (queue) {
        queue->front = -1;
        queue->rear = -1;
    }
    return queue;
}

int is_queue_empty(struct Queue* queue) {
    return queue && queue->front == -1;
}

void enqueue(struct Queue* queue, int value) {
    if (!queue || queue->rear == MAX_NODES - 1) return;
    
    if (queue->front == -1) {
        queue->front = 0;
    }
    queue->rear++;
    queue->items[queue->rear] = value;
}

int dequeue(struct Queue* queue) {
    if (!queue || is_queue_empty(queue)) return -1;
    
    int item = queue->items[queue->front];
    queue->front++;
    
    if (queue->front > queue->rear) {
        queue->front = queue->rear = -1;
    }
    return item;
}

void bfs(struct Graph* graph, int start) {
    if (!graph || !validate_node(graph, start)) return;
    
    int visited[MAX_NODES] = {0};
    struct Queue* queue = create_queue();
    if (!queue) return;
    
    visited[start] = 1;
    enqueue(queue, start);
    
    printf("BFS traversal: ");
    
    while (!is_queue_empty(queue)) {
        int current = dequeue(queue);
        if (current == -1) break;
        
        printf("%d ", current);
        
        for (int i = 0; i < graph->nodes; i++) {
            if (graph->matrix[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                enqueue(queue, i);
            }
        }
    }
    printf("\n");
    free(queue);
}

void dijkstra(struct Graph* graph, int start) {
    if (!graph || !validate_node(graph, start)) return;
    
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < graph->nodes; i++) {
        dist[i] = INF;
    }
    dist[start] = 0;
    
    for (int count = 0; count < graph->nodes - 1; count++) {
        int min_dist = INF;
        int min_index = -1;
        
        for (int i = 0; i < graph->nodes; i++) {
            if (!visited[i] && dist[i] <= min_dist) {
                min_dist = dist[i];
                min_index = i;
            }
        }
        
        if (min_index == -1) break;
        visited[min_index] = 1;
        
        for (int i = 0; i < graph->nodes; i++) {
            if (!visited[i] && graph->matrix[min_index][i] != 0 && 
                dist[min_index] != INF && 
                dist[min_index] + graph->matrix[min_index][i] < dist[i]) {
                dist[i] = dist[min_index] + graph->matrix[min_index][i];
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", start);
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
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || !validate_graph_size(nodes)) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    initialize_graph(&graph, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * (nodes - 1) / 2) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (source destination weight):\n");
    for (