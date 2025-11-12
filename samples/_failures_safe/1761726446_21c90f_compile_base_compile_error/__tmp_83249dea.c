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
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph->vertices[i].id = i;
        graph->vertices[i].visited = 0;
    }
}

int add_vertex(struct Graph* graph, int id) {
    if (graph->vertex_count >= MAX_VERTICES || id < 0 || id >= MAX_VERTICES) {
        return 0;
    }
    graph->vertices[id].id = id;
    graph->vertices[id].visited = 0;
    graph->vertex_count++;
    return 1;
}

int add_edge(struct Graph* graph, int src_id, int dest_id, int weight) {
    if (graph->edge_count >= MAX_EDGES || src_id < 0 || src_id >= MAX_VERTICES || 
        dest_id < 0 || dest_id >= MAX_VERTICES || weight < 0) {
        return 0;
    }
    graph->edges[graph->edge_count].src = &graph->vertices[src_id];
    graph->edges[graph->edge_count].dest = &graph->vertices[dest_id];
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
    if (new_node == NULL) return;
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
    if (is_queue_empty(queue)) return NULL;
    struct QueueNode* temp = queue->front;
    struct Vertex* vertex = temp->vertex;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    return vertex;
}

void bfs(struct Graph* graph, int start_id) {
    if (start_id < 0 || start_id >= MAX_VERTICES) return;
    for (int i = 0; i < graph->vertex_count; i++) {
        graph->vertices[i].visited = 0;
    }
    struct Queue queue;
    init_queue(&queue);
    graph->vertices[start_id].visited = 1;
    enqueue(&queue, &graph->vertices[start_id]);
    printf("BFS traversal: ");
    while (!is_queue_empty(&queue)) {
        struct Vertex* current = dequeue(&queue);
        printf("%d ", current->id);
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].src->id == current->id && !graph->edges[i].dest->visited) {
                graph->edges[i].dest->visited = 1;
                enqueue(&queue, graph->edges[i].dest);
            }
        }
    }
    printf("\n");
}

void dijkstra(struct Graph* graph, int start_id) {
    if (start_id < 0 || start_id >= MAX_VERTICES) return;
    int dist[MAX_VERTICES];
    int visited[MAX_VERTICES];
    for (int i = 0; i < MAX_VERTICES; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[start_id] = 0;
    for (int count = 0; count < graph->vertex_count - 1; count++) {
        int min_dist = INT_MAX;
        int min_index = -1;
        for (int v = 0; v < MAX_VERTICES; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_index = v;
            }
        }
        if (min_index == -1) break;
        visited[min_index] = 1;
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].src->id == min_index) {
                int dest_id = graph->edges[i].dest->id;
                int weight = graph->edges[i].weight;
                if (!visited[dest_id] && dist[min_index] != INT_MAX && 
                    dist[min_index] + weight < dist[dest_id]) {
                    dist[dest_id] = dist[min_index] + weight;
                }
            }
        }
    }
    printf("Dijk