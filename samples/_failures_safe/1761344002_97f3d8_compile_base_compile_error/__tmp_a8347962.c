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
    if (graph->edge_count >= MAX_EDGES) return 0;
    if (weight < 0) return 0;
    
    struct Vertex* src = NULL;
    struct Vertex* dest = NULL;
    
    for (int i = 0; i < graph->vertex_count; i++) {
        if (graph->vertices[i].id == src_id) src = &graph->vertices[i];
        if (graph->vertices[i].id == dest_id) dest = &graph->vertices[i];
    }
    
    if (src == NULL || dest == NULL) return 0;
    
    graph->edges[graph->edge_count].src = src;
    graph->edges[graph->edge_count].dest = dest;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void init_queue(struct Queue* q) {
    q->front = NULL;
    q->rear = NULL;
}

int is_queue_empty(struct Queue* q) {
    return q->front == NULL;
}

void enqueue(struct Queue* q, struct Vertex* vertex) {
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (new_node == NULL) return;
    
    new_node->vertex = vertex;
    new_node->next = NULL;
    
    if (q->rear == NULL) {
        q->front = new_node;
        q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
}

struct Vertex* dequeue(struct Queue* q) {
    if (is_queue_empty(q)) return NULL;
    
    struct QueueNode* temp = q->front;
    struct Vertex* vertex = temp->vertex;
    
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    
    free(temp);
    return vertex;
}

void reset_visited(struct Graph* graph) {
    for (int i = 0; i < graph->vertex_count; i++) {
        graph->vertices[i].visited = 0;
    }
}

void bfs(struct Graph* graph, int start_id) {
    reset_visited(graph);
    
    struct Vertex* start = NULL;
    for (int i = 0; i < graph->vertex_count; i++) {
        if (graph->vertices[i].id == start_id) {
            start = &graph->vertices[i];
            break;
        }
    }
    if (start == NULL) return;
    
    struct Queue q;
    init_queue(&q);
    
    start->visited = 1;
    enqueue(&q, start);
    
    printf("BFS traversal: ");
    
    while (!is_queue_empty(&q)) {
        struct Vertex* current = dequeue(&q);
        printf("%d ", current->id);
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].src == current && !graph->edges[i].dest->visited) {
                graph->edges[i].dest->visited = 1;
                enqueue(&q, graph->edges[i].dest);
            }
        }
    }
    printf("\n");
}

void dijkstra(struct Graph* graph, int start_id) {
    int dist[MAX_VERTICES];
    int visited[MAX_VERTICES] = {0};
    
    for (int i = 0; i < MAX_VERTICES; i++) {
        dist[i] = INT_MAX;
    }
    
    dist[start_id] = 0;
    
    for (int count = 0; count < graph->vertex_count; count++) {
        int min_dist = INT_MAX;
        int min_index = -1;
        
        for (int i = 0; i < graph->vertex_count; i++) {
            int id = graph->vertices[i].id;
            if (!visited[id] && dist[id] <= min_dist) {
                min_dist = dist[id];
                min_index = id;
            }
        }
        
        if (min_index == -1) break;
        visited[min_index] = 1;
        
        for (