//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct Node {
    int vertex;
    int weight;
    struct Node* next;
} Node;

typedef struct Graph {
    int num_vertices;
    Node* adjacency_list[MAX_NODES];
} Graph;

typedef struct HeapNode {
    int vertex;
    int distance;
} HeapNode;

typedef struct MinHeap {
    int size;
    int capacity;
    int* positions;
    HeapNode** array;
} MinHeap;

Graph* create_graph(int vertices) {
    if (vertices <= 0 || vertices > MAX_NODES) return NULL;
    Graph* graph = malloc(sizeof(Graph));
    if (!graph) return NULL;
    graph->num_vertices = vertices;
    for (int i = 0; i < vertices; i++) {
        graph->adjacency_list[i] = NULL;
    }
    return graph;
}

void add_edge(Graph* graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->num_vertices || dest < 0 || dest >= graph->num_vertices) return;
    Node* new_node = malloc(sizeof(Node));
    if (!new_node) return;
    new_node->vertex = dest;
    new_node->weight = weight;
    new_node->next = graph->adjacency_list[src];
    graph->adjacency_list[src] = new_node;
}

MinHeap* create_min_heap(int capacity) {
    if (capacity <= 0 || capacity > MAX_NODES) return NULL;
    MinHeap* heap = malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->positions = malloc(capacity * sizeof(int));
    if (!heap->positions) {
        free(heap);
        return NULL;
    }
    heap->array = malloc(capacity * sizeof(HeapNode*));
    if (!heap->array) {
        free(heap->positions);
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    for (int i = 0; i < capacity; i++) {
        heap->positions[i] = -1;
        heap->array[i] = NULL;
    }
    return heap;
}

void swap_heap_nodes(HeapNode** a, HeapNode** b) {
    HeapNode* temp = *a;
    *a = *b;
    *b = temp;
}

void min_heapify(MinHeap* heap, int idx) {
    if (!heap || idx < 0 || idx >= heap->size) return;
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < heap->size && heap->array[left]->distance < heap->array[smallest]->distance) {
        smallest = left;
    }
    if (right < heap->size && heap->array[right]->distance < heap->array[smallest]->distance) {
        smallest = right;
    }
    if (smallest != idx) {
        heap->positions[heap->array[smallest]->vertex] = idx;
        heap->positions[heap->array[idx]->vertex] = smallest;
        swap_heap_nodes(&heap->array[smallest], &heap->array[idx]);
        min_heapify(heap, smallest);
    }
}

HeapNode* extract_min(MinHeap* heap) {
    if (!heap || heap->size <= 0) return NULL;
    HeapNode* root = heap->array[0];
    HeapNode* last = heap->array[heap->size - 1];
    heap->array[0] = last;
    heap->positions[root->vertex] = -1;
    heap->positions[last->vertex] = 0;
    heap->size--;
    min_heapify(heap, 0);
    return root;
}

void decrease_key(MinHeap* heap, int vertex, int distance) {
    if (!heap || vertex < 0 || vertex >= heap->capacity) return;
    int i = heap->positions[vertex];
    if (i == -1 || heap->array[i]->distance <= distance) return;
    heap->array[i]->distance = distance;
    while (i > 0 && heap->array[(i - 1) / 2]->distance > heap->array[i]->distance) {
        heap->positions[heap->array[i]->vertex] = (i - 1) / 2;
        heap->positions[heap->array[(i - 1) / 2]->vertex] = i;
        swap_heap_nodes(&heap->array[i], &heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int is_in_heap(MinHeap* heap, int vertex) {
    if (!heap || vertex < 0 || vertex >= heap->capacity) return 0;
    return heap->positions[vertex] != -1;
}

void dijkstra(Graph* graph, int src) {
    if (!graph || src < 0 || src >= graph->num_vertices) return;
    int distances[MAX_NODES];
    MinHeap* heap = create_min_heap(graph->num_vertices);
    if (!heap) return;
    for (int v = 0; v < graph->num_vertices; v++) {
        distances[v] = INT_MAX;
        heap->array[v] = malloc(sizeof(HeapNode));
        if (heap->array[v]) {
            heap->array[v]->vertex = v;
            heap