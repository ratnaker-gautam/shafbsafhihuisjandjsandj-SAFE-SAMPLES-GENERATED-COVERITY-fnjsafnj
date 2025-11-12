//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 100
#define MAX_EDGES 4950

struct Vertex {
    int id;
    int visited;
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
    struct Vertex* vertex;
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
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph->vertices[i].id = i;
        graph->vertices[i].visited = 0;
    }
}

int add_vertex(struct Graph* graph, int vertex_id) {
    if (graph->vertex_count >= MAX_VERTICES || vertex_id < 0 || vertex_id >= MAX_VERTICES) {
        return 0;
    }
    if (graph->vertices[vertex_id].visited == 1) {
        return 0;
    }
    graph->vertices[vertex_id].visited = 0;
    graph->vertex_count++;
    return 1;
}

int add_edge(struct Graph* graph, int source_id, int dest_id, int weight) {
    if (source_id < 0 || source_id >= MAX_VERTICES || dest_id < 0 || dest_id >= MAX_VERTICES) {
        return 0;
    }
    if (graph->edge_count >= MAX_EDGES) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    graph->edges[graph->edge_count].source = &graph->vertices[source_id];
    graph->edges[graph->edge_count].destination = &graph->vertices[dest_id];
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void initialize_queue(struct Queue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}

int enqueue(struct Queue* queue, struct Vertex* vertex) {
    if (queue->size >= MAX_VERTICES) {
        return 0;
    }
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (new_node == NULL) {
        return 0;
    }
    new_node->vertex = vertex;
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

struct Vertex* dequeue(struct Queue* queue) {
    if (queue->front == NULL) {
        return NULL;
    }
    struct QueueNode* temp = queue->front;
    struct Vertex* vertex = temp->vertex;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    queue->size--;
    return vertex;
}

int is_queue_empty(struct Queue* queue) {
    return queue->front == NULL;
}

void bfs(struct Graph* graph, int start_vertex) {
    if (start_vertex < 0 || start_vertex >= MAX_VERTICES) {
        return;
    }
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph->vertices[i].visited = 0;
    }
    struct Queue queue;
    initialize_queue(&queue);
    graph->vertices[start_vertex].visited = 1;
    if (enqueue(&queue, &graph->vertices[start_vertex]) == 0) {
        return;
    }
    printf("BFS traversal: ");
    while (!is_queue_empty(&queue)) {
        struct Vertex* current = dequeue(&queue);
        if (current == NULL) {
            break;
        }
        printf("%d ", current->id);
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].source->id == current->id && graph->edges[i].destination->visited == 0) {
                graph->edges[i].destination->visited = 1;
                if (enqueue(&queue, graph->edges[i].destination) == 0) {
                    break;
                }
            }
        }
    }
    printf("\n");
}

void dijkstra(struct Graph* graph, int start_vertex) {
    if (start_vertex < 0 || start_vertex >= MAX_VERTICES) {
        return;
    }
    int dist[MAX_VERTICES];
    int visited[MAX_VERTICES];
    for (int i = 0; i < MAX_VERTICES; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[start_vertex] = 0;
    for (int count = 0; count < graph->vertex_count - 1; count++) {
        int min_dist = INT_MAX;
        int min_index = -1;
        for (int v = 0; v < MAX_VERTICES; v++) {
            if (visited[v] == 0 && dist[v] <= min_dist) {