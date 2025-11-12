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

void init_graph(Graph* graph, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) {
        exit(1);
    }
    graph->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        graph->lists[i].edges = malloc(sizeof(Edge) * 4);
        if (graph->lists[i].edges == NULL) {
            exit(1);
        }
        graph->lists[i].size = 0;
        graph->lists[i].capacity = 4;
    }
}

void add_edge(Graph* graph, int src, int dest, int weight) {
    if (src < 0 || src >= graph->node_count || dest < 0 || dest >= graph->node_count) {
        exit(1);
    }
    if (weight < 0) {
        exit(1);
    }
    AdjList* list = &graph->lists[src];
    if (list->size >= list->capacity) {
        int new_cap = list->capacity * 2;
        if (new_cap > MAX_NODES * 2) {
            exit(1);
        }
        Edge* new_edges = realloc(list->edges, sizeof(Edge) * new_cap);
        if (new_edges == NULL) {
            exit(1);
        }
        list->edges = new_edges;
        list->capacity = new_cap;
    }
    list->edges[list->size].dest = dest;
    list->edges[list->size].weight = weight;
    list->size++;
}

void init_heap(MinHeap* heap, int capacity) {
    if (capacity <= 0 || capacity > MAX_NODES) {
        exit(1);
    }
    heap->nodes = malloc(sizeof(HeapNode) * capacity);
    if (heap->nodes == NULL) {
        exit(1);
    }
    heap->size = 0;
    heap->capacity = capacity;
}

void heap_swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MinHeap* heap, int idx) {
    if (idx <= 0) return;
    int parent = (idx - 1) / 2;
    if (heap->nodes[parent].distance > heap->nodes[idx].distance) {
        heap_swap(&heap->nodes[parent], &heap->nodes[idx]);
        heapify_up(heap, parent);
    }
}

void heapify_down(MinHeap* heap, int idx) {
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    int smallest = idx;
    if (left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance) {
        smallest = left;
    }
    if (right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance) {
        smallest = right;
    }
    if (smallest != idx) {
        heap_swap(&heap->nodes[idx], &heap->nodes[smallest]);
        heapify_down(heap, smallest);
    }
}

void heap_push(MinHeap* heap, int node, int distance) {
    if (heap->size >= heap->capacity) {
        exit(1);
    }
    heap->nodes[heap->size].node = node;
    heap->nodes[heap->size].distance = distance;
    heapify_up(heap, heap->size);
    heap->size++;
}

HeapNode heap_pop(MinHeap* heap) {
    if (heap->size <= 0) {
        exit(1);
    }
    HeapNode root = heap->nodes[0];
    heap->size--;
    if (heap->size > 0) {
        heap->nodes[0] = heap->nodes[heap->size];
        heapify_down(heap, 0);
    }
    return root;
}

void dijkstra(Graph* graph, int start) {
    if (start < 0 || start >= graph->node_count) {
        exit(1);
    }
    int distances[MAX_NODES];
    for (int i = 0; i < graph->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    MinHeap heap;
    init_heap(&heap, graph->node_count);
    heap_push(&heap, start, 0);
    while (heap.size > 0) {
        HeapNode current = heap_pop(&heap);
        int u = current.node;
        AdjList* list = &graph->lists[u];
        for (int i = 0; i < list->size; i++) {
            int v = list->edges[i].dest;
            int weight = list->edges[i].weight;
            if (weight < 0) {
                exit(1);
            }
            if (distances[u] != INT_MAX) {
                long