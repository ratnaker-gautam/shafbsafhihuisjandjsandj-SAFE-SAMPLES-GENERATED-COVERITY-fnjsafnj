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
    struct Vertex* src;
    struct Vertex* dest;
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
};

void init_graph(struct Graph* graph) {
    graph->vertex_count = 0;
    graph->edge_count = 0;
}

int add_vertex(struct Graph* graph, int id) {
    if (graph->vertex_count >= MAX_VERTICES || id < 0) {
        return 0;
    }
    for (int i = 0; i < graph->vertex_count; i++) {
        if (graph->vertices[i].id == id) {
            return 0;
        }
    }
    graph->vertices[graph->vertex_count].id = id;
    graph->vertices[graph->vertex_count].visited = 0;
    graph->vertex_count++;
    return 1;
}

int add_edge(struct Graph* graph, int src_id, int dest_id, int weight) {
    if (graph->edge_count >= MAX_EDGES || weight < 0) {
        return 0;
    }
    struct Vertex* src = NULL;
    struct Vertex* dest = NULL;
    for (int i = 0; i < graph->vertex_count; i++) {
        if (graph->vertices[i].id == src_id) {
            src = &graph->vertices[i];
        }
        if (graph->vertices[i].id == dest_id) {
            dest = &graph->vertices[i];
        }
    }
    if (src == NULL || dest == NULL) {
        return 0;
    }
    graph->edges[graph->edge_count].src = src;
    graph->edges[graph->edge_count].dest = dest;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void init_queue(struct Queue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
}

int is_queue_empty(struct Queue* queue) {
    return queue->front == NULL;
}

void enqueue(struct Queue* queue, struct Vertex* vertex) {
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (new_node == NULL) {
        return;
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
}

struct Vertex* dequeue(struct Queue* queue) {
    if (is_queue_empty(queue)) {
        return NULL;
    }
    struct QueueNode* temp = queue->front;
    struct Vertex* vertex = temp->vertex;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    return vertex;
}

void reset_visited(struct Graph* graph) {
    for (int i = 0; i < graph->vertex_count; i++) {
        graph->vertices[i].visited = 0;
    }
}

struct Vertex* find_vertex(struct Graph* graph, int id) {
    for (int i = 0; i < graph->vertex_count; i++) {
        if (graph->vertices[i].id == id) {
            return &graph->vertices[i];
        }
    }
    return NULL;
}

void bfs(struct Graph* graph, int start_id) {
    struct Vertex* start = find_vertex(graph, start_id);
    if (start == NULL) {
        return;
    }
    reset_visited(graph);
    struct Queue queue;
    init_queue(&queue);
    start->visited = 1;
    enqueue(&queue, start);
    printf("BFS traversal: ");
    while (!is_queue_empty(&queue)) {
        struct Vertex* current = dequeue(&queue);
        printf("%d ", current->id);
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].src == current && !graph->edges[i].dest->visited) {
                graph->edges[i].dest->visited = 1;
                enqueue(&queue, graph->edges[i].dest);
            }
        }
    }
    printf("\n");
    while (!is_queue_empty(&queue)) {
        dequeue(&queue);
    }
}

void dijkstra(struct Graph* graph, int start_id) {
    struct Vertex* start = find_vertex(graph, start_id);
    if (start == NULL) {
        return;
    }
    int dist[MAX_VERTICES];
    int visited[MAX_VERTICES];
    for (int i = 0; i < graph->vertex_count; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    int start_index = -1;
    for (int i = 0; i < graph->vertex_count; i++) {
        if (&graph->vertices[i] == start) {
            start_index = i;
            break;
        }
    }
    if (start_index == -1) {