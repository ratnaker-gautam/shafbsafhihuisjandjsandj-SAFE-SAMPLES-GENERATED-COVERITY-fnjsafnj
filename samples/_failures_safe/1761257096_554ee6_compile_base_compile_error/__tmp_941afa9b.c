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
    graph->vertex_count = 0;
    graph->edge_count = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph->vertices[i].id = i;
        graph->vertices[i].visited = 0;
    }
}

int add_vertex(struct Graph* graph, int id) {
    if (graph->vertex_count >= MAX_VERTICES || id < 0 || id >= MAX_VERTICES) {
        return -1;
    }
    graph->vertices[graph->vertex_count].id = id;
    graph->vertices[graph->vertex_count].visited = 0;
    graph->vertex_count++;
    return 0;
}

int add_edge(struct Graph* graph, int from_id, int to_id, int weight) {
    if (graph->edge_count >= MAX_EDGES || from_id < 0 || from_id >= graph->vertex_count || 
        to_id < 0 || to_id >= graph->vertex_count || weight < 0) {
        return -1;
    }
    graph->edges[graph->edge_count].from = &graph->vertices[from_id];
    graph->edges[graph->edge_count].to = &graph->vertices[to_id];
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 0;
}

void init_queue(struct Queue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}

int enqueue(struct Queue* queue, struct Vertex* vertex) {
    if (queue->size >= MAX_VERTICES) {
        return -1;
    }
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) {
        return -1;
    }
    new_node->vertex = vertex;
    new_node->next = NULL;
    if (queue->rear) {
        queue->rear->next = new_node;
    }
    queue->rear = new_node;
    if (!queue->front) {
        queue->front = new_node;
    }
    queue->size++;
    return 0;
}

struct Vertex* dequeue(struct Queue* queue) {
    if (!queue->front) {
        return NULL;
    }
    struct QueueNode* temp = queue->front;
    struct Vertex* vertex = temp->vertex;
    queue->front = queue->front->next;
    if (!queue->front) {
        queue->rear = NULL;
    }
    free(temp);
    queue->size--;
    return vertex;
}

int is_queue_empty(struct Queue* queue) {
    return queue->front == NULL;
}

void reset_visited(struct Graph* graph) {
    for (int i = 0; i < graph->vertex_count; i++) {
        graph->vertices[i].visited = 0;
    }
}

void bfs(struct Graph* graph, int start_id) {
    if (start_id < 0 || start_id >= graph->vertex_count) {
        return;
    }
    reset_visited(graph);
    struct Queue queue;
    init_queue(&queue);
    graph->vertices[start_id].visited = 1;
    enqueue(&queue, &graph->vertices[start_id]);
    printf("BFS traversal: ");
    while (!is_queue_empty(&queue)) {
        struct Vertex* current = dequeue(&queue);
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
    if (start_id < 0 || start_id >= graph->vertex_count) {
        return;
    }
    int dist[MAX_VERTICES];
    int visited[MAX_VERTICES];
    for (int i = 0; i < graph->vertex_count; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[start_id] = 0;
    for (int count = 0; count < graph->vertex_count - 1; count++) {
        int min_dist = INT_MAX;
        int min_index = -1;
        for (int v = 0; v < graph->vertex_count; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_index = v;
            }
        }
        if (min_index == -1) {
            break;
        }
        visited[min_index] = 1;
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].from->id == min_index && !visited[graph