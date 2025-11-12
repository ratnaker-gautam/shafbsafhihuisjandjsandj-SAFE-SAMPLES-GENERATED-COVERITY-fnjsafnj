//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 100
#define MAX_EDGES 1000

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
    int size;
};

void init_queue(struct Queue* q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

int is_queue_empty(struct Queue* q) {
    return q->size == 0;
}

void enqueue(struct Queue* q, struct Vertex* v) {
    if (q->size >= MAX_VERTICES) return;
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return;
    new_node->vertex = v;
    new_node->next = NULL;
    if (q->rear) {
        q->rear->next = new_node;
    }
    q->rear = new_node;
    if (!q->front) {
        q->front = new_node;
    }
    q->size++;
}

struct Vertex* dequeue(struct Queue* q) {
    if (is_queue_empty(q)) return NULL;
    struct QueueNode* temp = q->front;
    struct Vertex* v = temp->vertex;
    q->front = q->front->next;
    if (!q->front) {
        q->rear = NULL;
    }
    free(temp);
    q->size--;
    return v;
}

void init_graph(struct Graph* g) {
    g->vertex_count = 0;
    g->edge_count = 0;
}

int add_vertex(struct Graph* g, int id) {
    if (g->vertex_count >= MAX_VERTICES) return 0;
    if (id < 0 || id >= MAX_VERTICES) return 0;
    for (int i = 0; i < g->vertex_count; i++) {
        if (g->vertices[i].id == id) return 0;
    }
    g->vertices[g->vertex_count].id = id;
    g->vertices[g->vertex_count].visited = 0;
    g->vertex_count++;
    return 1;
}

int add_edge(struct Graph* g, int src_id, int dest_id, int weight) {
    if (g->edge_count >= MAX_EDGES) return 0;
    if (src_id < 0 || src_id >= MAX_VERTICES) return 0;
    if (dest_id < 0 || dest_id >= MAX_VERTICES) return 0;
    if (weight < 0) return 0;
    struct Vertex* src = NULL;
    struct Vertex* dest = NULL;
    for (int i = 0; i < g->vertex_count; i++) {
        if (g->vertices[i].id == src_id) src = &g->vertices[i];
        if (g->vertices[i].id == dest_id) dest = &g->vertices[i];
    }
    if (!src || !dest) return 0;
    g->edges[g->edge_count].src = src;
    g->edges[g->edge_count].dest = dest;
    g->edges[g->edge_count].weight = weight;
    g->edge_count++;
    return 1;
}

void reset_visited(struct Graph* g) {
    for (int i = 0; i < g->vertex_count; i++) {
        g->vertices[i].visited = 0;
    }
}

void bfs(struct Graph* g, int start_id) {
    if (g->vertex_count == 0) return;
    reset_visited(g);
    struct Vertex* start = NULL;
    for (int i = 0; i < g->vertex_count; i++) {
        if (g->vertices[i].id == start_id) {
            start = &g->vertices[i];
            break;
        }
    }
    if (!start) return;
    struct Queue q;
    init_queue(&q);
    start->visited = 1;
    enqueue(&q, start);
    printf("BFS traversal: ");
    while (!is_queue_empty(&q)) {
        struct Vertex* current = dequeue(&q);
        printf("%d ", current->id);
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].src == current && !g->edges[i].dest->visited) {
                g->edges[i].dest->visited = 1;
                enqueue(&q, g->edges[i].dest);
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph g;
    init_graph(&g);
    int vertex_count;
    printf("Enter number of vertices (1-100): ");
    if (scanf("%d", &vertex_count) != 1 || vertex_count < 1 || vertex_count > MAX_VERTICES) {
        printf("Invalid vertex count\n");
        return 1;
    }
    for (int i = 0; i < vertex_count; i++) {
        int id;
        printf("Enter vertex ID %d: ", i);
        if (scanf("%d", &id) != 1 || id < 0 || id >=