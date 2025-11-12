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
    struct Vertex* from;
    struct Vertex* to;
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
    if (!graph) return;
    graph->vertex_count = 0;
    graph->edge_count = 0;
}

int add_vertex(struct Graph* graph, int id) {
    if (!graph) return 0;
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

int add_edge(struct Graph* graph, int from_id, int to_id, int weight) {
    if (!graph) return 0;
    if (graph->edge_count >= MAX_EDGES) return 0;
    if (weight < 0) return 0;
    
    struct Vertex* from_vertex = NULL;
    struct Vertex* to_vertex = NULL;
    
    for (int i = 0; i < graph->vertex_count; i++) {
        if (graph->vertices[i].id == from_id) from_vertex = &graph->vertices[i];
        if (graph->vertices[i].id == to_id) to_vertex = &graph->vertices[i];
    }
    
    if (!from_vertex || !to_vertex) return 0;
    
    graph->edges[graph->edge_count].from = from_vertex;
    graph->edges[graph->edge_count].to = to_vertex;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void init_queue(struct Queue* queue) {
    if (!queue) return;
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}

int enqueue(struct Queue* queue, struct Vertex* vertex) {
    if (!queue || !vertex) return 0;
    
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return 0;
    
    new_node->vertex = vertex;
    new_node->next = NULL;
    
    if (queue->rear) {
        queue->rear->next = new_node;
    } else {
        queue->front = new_node;
    }
    queue->rear = new_node;
    queue->size++;
    return 1;
}

struct Vertex* dequeue(struct Queue* queue) {
    if (!queue || !queue->front) return NULL;
    
    struct QueueNode* temp = queue->front;
    struct Vertex* vertex = temp->vertex;
    
    queue->front = queue->front->next;
    if (!queue->front) queue->rear = NULL;
    
    free(temp);
    queue->size--;
    return vertex;
}

int is_queue_empty(struct Queue* queue) {
    if (!queue) return 1;
    return queue->front == NULL;
}

void clear_visited(struct Graph* graph) {
    if (!graph) return;
    for (int i = 0; i < graph->vertex_count; i++) {
        graph->vertices[i].visited = 0;
    }
}

void bfs(struct Graph* graph, int start_id) {
    if (!graph || graph->vertex_count == 0) return;
    
    struct Vertex* start_vertex = NULL;
    for (int i = 0; i < graph->vertex_count; i++) {
        if (graph->vertices[i].id == start_id) {
            start_vertex = &graph->vertices[i];
            break;
        }
    }
    if (!start_vertex) return;
    
    clear_visited(graph);
    struct Queue queue;
    init_queue(&queue);
    
    start_vertex->visited = 1;
    enqueue(&queue, start_vertex);
    
    printf("BFS traversal: ");
    while (!is_queue_empty(&queue)) {
        struct Vertex* current = dequeue(&queue);
        if (!current) continue;
        printf("%d ", current->id);
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].from == current && !graph->edges[i].to->visited) {
                graph->edges[i].to->visited = 1;
                enqueue(&queue, graph->edges[i].to);
            }
        }
    }
    printf("\n");
}

void dijkstra(struct Graph* graph, int start_id) {
    if (!graph || graph->vertex_count == 0) return;
    
    int distances[MAX_VERTICES];
    int visited[MAX_VERTICES];
    
    for (int i = 0; i < MAX_VERTICES; i++) {
        distances[i] = INT_MAX;
        visited[i]