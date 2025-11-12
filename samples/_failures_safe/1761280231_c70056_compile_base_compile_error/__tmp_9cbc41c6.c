//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Edge {
    int src;
    int dest;
    int weight;
};

struct Graph {
    int num_nodes;
    int num_edges;
    struct Edge edges[MAX_EDGES];
};

struct MinHeapNode {
    int vertex;
    int distance;
};

struct MinHeap {
    int size;
    int capacity;
    int *pos;
    struct MinHeapNode **array;
};

struct MinHeapNode* new_min_heap_node(int vertex, int distance) {
    struct MinHeapNode* node = malloc(sizeof(struct MinHeapNode));
    if (node == NULL) return NULL;
    node->vertex = vertex;
    node->distance = distance;
    return node;
}

struct MinHeap* create_min_heap(int capacity) {
    if (capacity <= 0 || capacity > MAX_NODES) return NULL;
    struct MinHeap* heap = malloc(sizeof(struct MinHeap));
    if (heap == NULL) return NULL;
    heap->pos = malloc(capacity * sizeof(int));
    if (heap->pos == NULL) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    heap->array = malloc(capacity * sizeof(struct MinHeapNode*));
    if (heap->array == NULL) {
        free(heap->pos);
        free(heap);
        return NULL;
    }
    return heap;
}

void swap_min_heap_node(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* temp = *a;
    *a = *b;
    *b = temp;
}

void min_heapify(struct MinHeap* heap, int idx) {
    if (heap == NULL) return;
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
        heap->pos[heap->array[smallest]->vertex] = idx;
        heap->pos[heap->array[idx]->vertex] = smallest;
        swap_min_heap_node(&heap->array[smallest], &heap->array[idx]);
        min_heapify(heap, smallest);
    }
}

int is_empty(struct MinHeap* heap) {
    if (heap == NULL) return 1;
    return heap->size == 0;
}

struct MinHeapNode* extract_min(struct MinHeap* heap) {
    if (heap == NULL || heap->size <= 0) return NULL;
    if (heap->size == 1) {
        heap->size--;
        return heap->array[0];
    }
    struct MinHeapNode* root = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->pos[root->vertex] = heap->size - 1;
    heap->pos[heap->array[0]->vertex] = 0;
    heap->size--;
    min_heapify(heap, 0);
    return root;
}

void decrease_key(struct MinHeap* heap, int vertex, int distance) {
    if (heap == NULL) return;
    int i = heap->pos[vertex];
    heap->array[i]->distance = distance;
    while (i && heap->array[i]->distance < heap->array[(i - 1) / 2]->distance) {
        heap->pos[heap->array[i]->vertex] = (i - 1) / 2;
        heap->pos[heap->array[(i - 1) / 2]->vertex] = i;
        swap_min_heap_node(&heap->array[i], &heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int is_in_min_heap(struct MinHeap* heap, int vertex) {
    if (heap == NULL) return 0;
    if (vertex < 0 || vertex >= heap->capacity) return 0;
    return heap->pos[vertex] < heap->size;
}

void dijkstra(struct Graph* graph, int src) {
    if (graph == NULL || src < 0 || src >= graph->num_nodes) return;
    int distances[MAX_NODES];
    for (int i = 0; i < graph->num_nodes; i++) {
        distances[i] = INT_MAX;
    }
    distances[src] = 0;
    struct MinHeap* heap = create_min_heap(graph->num_nodes);
    if (heap == NULL) return;
    for (int v = 0; v < graph->num_nodes; v++) {
        heap->array[v] = new_min_heap_node(v, distances[v]);
        if (heap->array[v] == NULL) {
            for (int j = 0; j < v; j++) free(heap->array[j]);
            free(heap->array);
            free(heap->pos);
            free(heap);
            return;
        }
        heap->pos[v] = v;
    }
    heap->size = graph->num_nodes;
    while (!is_empty(heap)) {
        struct MinHeapNode* node = extract_min(