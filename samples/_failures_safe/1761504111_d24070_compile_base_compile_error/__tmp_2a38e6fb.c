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

void init_graph(struct Graph* graph) {
    if (graph == NULL) return;
    graph->vertex_count = 0;
    graph->edge_count = 0;
}

int add_vertex(struct Graph* graph, int id) {
    if (graph == NULL) return 0;
    if (graph->vertex_count >= MAX_VERTICES) return 0;
    if (id < 0 || id >= MAX_VERTICES) return 0;
    
    for (int i = 0; i < graph->vertex_count; i++) {
        if (graph->vertices[i].id == id) return 0;
    }
    
    graph->vertices[graph->vertex_count].id = id;
    graph->vertices[graph->vertex_count].visited = 0;
    graph->vertex_count++;
    return 1;
}

int add_edge(struct Graph* graph, int src_id, int dest_id, int weight) {
    if (graph == NULL) return 0;
    if (graph->edge_count >= MAX_EDGES) return 0;
    if (src_id < 0 || src_id >= MAX_VERTICES) return 0;
    if (dest_id < 0 || dest_id >= MAX_VERTICES) return 0;
    if (weight < 0) return 0;
    
    struct Vertex* src_vertex = NULL;
    struct Vertex* dest_vertex = NULL;
    
    for (int i = 0; i < graph->vertex_count; i++) {
        if (graph->vertices[i].id == src_id) src_vertex = &graph->vertices[i];
        if (graph->vertices[i].id == dest_id) dest_vertex = &graph->vertices[i];
    }
    
    if (src_vertex == NULL || dest_vertex == NULL) return 0;
    
    graph->edges[graph->edge_count].source = src_vertex;
    graph->edges[graph->edge_count].destination = dest_vertex;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void init_queue(struct Queue* queue) {
    if (queue == NULL) return;
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}

int enqueue(struct Queue* queue, struct Vertex* vertex) {
    if (queue == NULL || vertex == NULL) return 0;
    if (queue->size >= MAX_VERTICES) return 0;
    
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (new_node == NULL) return 0;
    
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
    if (queue == NULL || queue->front == NULL) return NULL;
    
    struct QueueNode* temp = queue->front;
    struct Vertex* vertex = temp->vertex;
    
    queue->front = queue->front->next;
    if (queue->front == NULL) queue->rear = NULL;
    
    free(temp);
    queue->size--;
    return vertex;
}

int is_queue_empty(struct Queue* queue) {
    if (queue == NULL) return 1;
    return queue->front == NULL;
}

void reset_visited(struct Graph* graph) {
    if (graph == NULL) return;
    for (int i = 0; i < graph->vertex_count; i++) {
        graph->vertices[i].visited = 0;
    }
}

void dfs_util(struct Vertex* vertex, struct Graph* graph) {
    if (vertex == NULL || graph == NULL) return;
    
    vertex->visited = 1;
    printf("%d ", vertex->id);
    
    for (int i = 0; i < graph->edge_count; i++) {
        if (graph->edges[i].source == vertex && !graph->edges[i].destination->visited) {
            dfs_util(graph->edges[i].destination, graph);
        }
    }
}

void dfs(struct Graph* graph, int start_id) {
    if (graph == NULL) return;
    
    reset_visited(graph);
    
    struct Vertex* start_vertex = NULL;
    for (int i = 0; i < graph->vertex_count; i++) {
        if (graph->vertices[i].id == start_id) {
            start_vertex = &graph->vertices[i];
            break;
        }
    }
    
    if (start_vertex == NULL) return;
    
    printf("DFS traversal: ");
    dfs_util(start_vertex, graph);
    printf("\n");
}

void bfs(struct Graph* graph, int start_id) {
    if (