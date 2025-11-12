//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int dest;
    int weight;
} Edge;

typedef struct {
    Edge* edges;
    int size;
    int capacity;
} AdjList;

typedef struct {
    AdjList lists[MAX_NODES];
    int num_nodes;
} Graph;

typedef struct {
    int node;
    int distance;
} HeapNode;

typedef struct {
    HeapNode* nodes;
    int size;
    int capacity;
} MinHeap;

typedef struct {
    int items[MAX_NODES];
    int front;
    int rear;
} Queue;

void init_graph(Graph* graph, int num_nodes) {
    if (num_nodes <= 0 || num_nodes > MAX_NODES) {
        num_nodes = MAX_NODES;
    }
    graph->num_nodes = num_nodes;
    for (int i = 0; i < num_nodes; i++) {
        graph->lists[i].edges = NULL;
        graph->lists[i].size = 0;
        graph->lists[i].capacity = 0;
    }
}

void add_edge(Graph* graph, int src, int dest, int weight) {
    if (src < 0 || src >= graph->num_nodes || dest < 0 || dest >= graph->num_nodes) {
        return;
    }
    if (weight < 0) {
        weight = 0;
    }
    
    AdjList* list = &graph->lists[src];
    if (list->size >= list->capacity) {
        int new_capacity = list->capacity == 0 ? 4 : list->capacity * 2;
        if (new_capacity > MAX_NODES * 2) {
            new_capacity = MAX_NODES * 2;
        }
        Edge* new_edges = realloc(list->edges, new_capacity * sizeof(Edge));
        if (new_edges) {
            list->edges = new_edges;
            list->capacity = new_capacity;
        } else {
            return;
        }
    }
    
    list->edges[list->size].dest = dest;
    list->edges[list->size].weight = weight;
    list->size++;
}

void free_graph(Graph* graph) {
    for (int i = 0; i < graph->num_nodes; i++) {
        if (graph->lists[i].edges) {
            free(graph->lists[i].edges);
        }
    }
}

MinHeap* create_heap(int capacity) {
    if (capacity <= 0) {
        capacity = MAX_NODES;
    }
    MinHeap* heap = malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->nodes = malloc(capacity * sizeof(HeapNode));
    if (!heap->nodes) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void heapify_up(MinHeap* heap, int index) {
    if (index <= 0) return;
    int parent = (index - 1) / 2;
    if (heap->nodes[index].distance < heap->nodes[parent].distance) {
        HeapNode temp = heap->nodes[index];
        heap->nodes[index] = heap->nodes[parent];
        heap->nodes[parent] = temp;
        heapify_up(heap, parent);
    }
}

void heapify_down(MinHeap* heap, int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int smallest = index;
    
    if (left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance) {
        smallest = left;
    }
    if (right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance) {
        smallest = right;
    }
    
    if (smallest != index) {
        HeapNode temp = heap->nodes[index];
        heap->nodes[index] = heap->nodes[smallest];
        heap->nodes[smallest] = temp;
        heapify_down(heap, smallest);
    }
}

void heap_push(MinHeap* heap, int node, int distance) {
    if (heap->size >= heap->capacity) return;
    heap->nodes[heap->size].node = node;
    heap->nodes[heap->size].distance = distance;
    heap->size++;
    heapify_up(heap, heap->size - 1);
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode empty = {-1, INT_MAX};
    if (heap->size <= 0) return empty;
    
    HeapNode result = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return result;
}

void init_queue(Queue* q) {
    q->front = 0;
    q->rear = 0;
}

int is_queue_empty(Queue* q) {
    return q->front == q->rear;
}

void enqueue(Queue* q, int item) {
    if ((q->rear + 1) % MAX_NODES == q->front) return;
    q->items[q->rear] = item;
    q->rear = (q->rear + 1) % MAX_NODES;
}

int dequeue(Queue* q) {
    if (is_queue_empty(q)) return -1;
    int item = q