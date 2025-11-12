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
        return;
    }
    if (weight < 0) {
        return;
    }
    
    AdjList* list = &graph->lists[src];
    if (list->size >= list->capacity) {
        int new_capacity = list->capacity * 2;
        if (new_capacity > MAX_NODES * 2) {
            return;
        }
        Edge* new_edges = realloc(list->edges, sizeof(Edge) * new_capacity);
        if (new_edges == NULL) {
            return;
        }
        list->edges = new_edges;
        list->capacity = new_capacity;
    }
    
    list->edges[list->size].dest = dest;
    list->edges[list->size].weight = weight;
    list->size++;
}

MinHeap* create_minheap(int capacity) {
    if (capacity <= 0) {
        return NULL;
    }
    MinHeap* heap = malloc(sizeof(MinHeap));
    if (heap == NULL) {
        return NULL;
    }
    heap->nodes = malloc(sizeof(HeapNode) * capacity);
    if (heap->nodes == NULL) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void heapify_up(MinHeap* heap, int index) {
    if (heap == NULL || index < 0 || index >= heap->size) {
        return;
    }
    int parent = (index - 1) / 2;
    while (index > 0 && heap->nodes[parent].distance > heap->nodes[index].distance) {
        HeapNode temp = heap->nodes[parent];
        heap->nodes[parent] = heap->nodes[index];
        heap->nodes[index] = temp;
        index = parent;
        parent = (index - 1) / 2;
    }
}

void heapify_down(MinHeap* heap, int index) {
    if (heap == NULL || index < 0 || index >= heap->size) {
        return;
    }
    while (1) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;
        
        if (left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance) {
            smallest = left;
        }
        if (right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance) {
            smallest = right;
        }
        if (smallest == index) {
            break;
        }
        HeapNode temp = heap->nodes[index];
        heap->nodes[index] = heap->nodes[smallest];
        heap->nodes[smallest] = temp;
        index = smallest;
    }
}

void heap_push(MinHeap* heap, int node, int distance) {
    if (heap == NULL || heap->size >= heap->capacity) {
        return;
    }
    heap->nodes[heap->size].node = node;
    heap->nodes[heap->size].distance = distance;
    heap->size++;
    heapify_up(heap, heap->size - 1);
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode empty = {-1, INT_MAX};
    if (heap == NULL || heap->size == 0) {
        return empty;
    }
    HeapNode result = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return result;
}

void dijkstra(Graph* graph, int start) {
    if (graph == NULL || start < 0 || start >= graph->node_count) {
        return;
    }
    
    int distances[MAX_NODES];
    for (int i = 0; i < graph->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    MinHeap* heap = create_minheap(graph->node_count);
    if (heap == NULL) {
        return;
    }
    heap_push(heap, start, 0);
    
    while (heap->size > 0) {