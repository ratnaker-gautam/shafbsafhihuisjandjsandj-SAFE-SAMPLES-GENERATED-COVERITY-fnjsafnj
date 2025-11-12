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
    int node_count;
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
    int size;
} Queue;

void init_graph(Graph* graph, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) {
        node_count = MAX_NODES;
    }
    graph->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        graph->lists[i].edges = NULL;
        graph->lists[i].size = 0;
        graph->lists[i].capacity = 0;
    }
}

void add_edge(Graph* graph, int src, int dest, int weight) {
    if (src < 0 || src >= graph->node_count || dest < 0 || dest >= graph->node_count) {
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

void init_heap(MinHeap* heap, int capacity) {
    if (capacity <= 0) {
        capacity = MAX_NODES;
    }
    heap->nodes = malloc(capacity * sizeof(HeapNode));
    if (heap->nodes) {
        heap->size = 0;
        heap->capacity = capacity;
    } else {
        heap->capacity = 0;
        heap->size = 0;
    }
}

void heap_swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MinHeap* heap, int index) {
    if (index <= 0) return;
    int parent = (index - 1) / 2;
    if (heap->nodes[index].distance < heap->nodes[parent].distance) {
        heap_swap(&heap->nodes[index], &heap->nodes[parent]);
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
        heap_swap(&heap->nodes[index], &heap->nodes[smallest]);
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
    q->rear = -1;
    q->size = 0;
}

void enqueue(Queue* q, int item) {
    if (q->size >= MAX_NODES) return;
    q->rear = (q->rear + 1) % MAX_NODES;
    q->items[q->rear] = item;
    q->size++;
}

int dequeue(Queue* q) {
    if (q->size <= 0) return -1;
    int item = q->items[q->front];
    q->front = (q->front + 1) % MAX_NODES;
    q->size--;
    return item;
}

void dijkstra(Graph* graph, int start) {
    if (start < 0 || start >= graph->node_count) return;
    
    int distances