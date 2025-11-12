//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 100
#define MAX_EDGES 4950

struct Vertex {
    int id;
    int degree;
};

struct Edge {
    struct Vertex* source;
    struct Vertex* destination;
    int weight;
};

struct Graph {
    struct Vertex vertices[MAX_VERTICES];
    struct Edge edges[MAX_EDGES];
    int vertex_count;
    int edge_count;
};

struct QueueNode {
    int vertex_id;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
    int size;
};

void initialize_graph(struct Graph* graph) {
    graph->vertex_count = 0;
    graph->edge_count = 0;
}

int add_vertex(struct Graph* graph, int id) {
    if (graph->vertex_count >= MAX_VERTICES || id < 0 || id >= MAX_VERTICES) {
        return 0;
    }
    for (int i = 0; i < graph->vertex_count; i++) {
        if (graph->vertices[i].id == id) {
            return 0;
        }
    }
    graph->vertices[graph->vertex_count].id = id;
    graph->vertices[graph->vertex_count].degree = 0;
    graph->vertex_count++;
    return 1;
}

int add_edge(struct Graph* graph, int source_id, int dest_id, int weight) {
    if (graph->edge_count >= MAX_EDGES || weight < 0) {
        return 0;
    }
    struct Vertex* source = NULL;
    struct Vertex* dest = NULL;
    for (int i = 0; i < graph->vertex_count; i++) {
        if (graph->vertices[i].id == source_id) {
            source = &graph->vertices[i];
        }
        if (graph->vertices[i].id == dest_id) {
            dest = &graph->vertices[i];
        }
    }
    if (source == NULL || dest == NULL) {
        return 0;
    }
    graph->edges[graph->edge_count].source = source;
    graph->edges[graph->edge_count].destination = dest;
    graph->edges[graph->edge_count].weight = weight;
    source->degree++;
    dest->degree++;
    graph->edge_count++;
    return 1;
}

void initialize_queue(struct Queue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}

int enqueue(struct Queue* queue, int vertex_id) {
    if (queue->size >= MAX_VERTICES) {
        return 0;
    }
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (new_node == NULL) {
        return 0;
    }
    new_node->vertex_id = vertex_id;
    new_node->next = NULL;
    if (queue->rear == NULL) {
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
    queue->size++;
    return 1;
}

int dequeue(struct Queue* queue) {
    if (queue->front == NULL) {
        return -1;
    }
    struct QueueNode* temp = queue->front;
    int vertex_id = temp->vertex_id;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    queue->size--;
    return vertex_id;
}

int is_queue_empty(struct Queue* queue) {
    return queue->front == NULL;
}

void bfs(struct Graph* graph, int start_id) {
    if (graph->vertex_count == 0) {
        return;
    }
    int visited[MAX_VERTICES] = {0};
    struct Queue queue;
    initialize_queue(&queue);
    if (!enqueue(&queue, start_id)) {
        return;
    }
    visited[start_id] = 1;
    printf("BFS traversal: ");
    while (!is_queue_empty(&queue)) {
        int current_id = dequeue(&queue);
        if (current_id == -1) {
            break;
        }
        printf("%d ", current_id);
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].source->id == current_id && !visited[graph->edges[i].destination->id]) {
                if (enqueue(&queue, graph->edges[i].destination->id)) {
                    visited[graph->edges[i].destination->id] = 1;
                }
            }
        }
    }
    printf("\n");
}

void dijkstra(struct Graph* graph, int start_id) {
    if (graph->vertex_count == 0) {
        return;
    }
    int dist[MAX_VERTICES];
    int visited[MAX_VERTICES] = {0};
    for (int i = 0; i < MAX_VERTICES; i++) {
        dist[i] = INT_MAX;
    }
    dist[start_id] = 0;
    for (int count = 0; count < graph->vertex_count; count++) {
        int min_dist = INT_MAX;
        int min_index = -1;
        for (int v = 0; v < MAX_VERTICES; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_index = v;
            }
        }
        if (min_index ==