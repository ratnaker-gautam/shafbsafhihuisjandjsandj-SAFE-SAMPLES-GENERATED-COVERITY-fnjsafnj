//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Edge {
    int src;
    int dest;
    int weight;
};

struct Graph {
    int num_vertices;
    int num_edges;
    struct Edge* edges;
};

struct MinHeapNode {
    int vertex;
    int distance;
};

struct MinHeap {
    int size;
    int capacity;
    int* pos;
    struct MinHeapNode** array;
};

struct Graph* create_graph(int vertices, int edges) {
    if (vertices <= 0 || edges < 0) return NULL;
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    graph->num_vertices = vertices;
    graph->num_edges = edges;
    graph->edges = malloc(edges * sizeof(struct Edge));
    if (!graph->edges) {
        free(graph);
        return NULL;
    }
    return graph;
}

struct MinHeapNode* new_min_heap_node(int vertex, int distance) {
    struct MinHeapNode* node = malloc(sizeof(struct MinHeapNode));
    if (!node) return NULL;
    node->vertex = vertex;
    node->distance = distance;
    return node;
}

struct MinHeap* create_min_heap(int capacity) {
    if (capacity <= 0) return NULL;
    struct MinHeap* heap = malloc(sizeof(struct MinHeap));
    if (!heap) return NULL;
    heap->pos = malloc(capacity * sizeof(int));
    if (!heap->pos) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    heap->array = malloc(capacity * sizeof(struct MinHeapNode*));
    if (!heap->array) {
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
    if (!heap || idx < 0 || idx >= heap->size) return;
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < heap->size && heap->array[left]->distance < heap->array[smallest]->distance) smallest = left;
    if (right < heap->size && heap->array[right]->distance < heap->array[smallest]->distance) smallest = right;
    if (smallest != idx) {
        struct MinHeapNode* smallest_node = heap->array[smallest];
        struct MinHeapNode* idx_node = heap->array[idx];
        heap->pos[smallest_node->vertex] = idx;
        heap->pos[idx_node->vertex] = smallest;
        swap_min_heap_node(&heap->array[smallest], &heap->array[idx]);
        min_heapify(heap, smallest);
    }
}

struct MinHeapNode* extract_min(struct MinHeap* heap) {
    if (!heap || heap->size <= 0) return NULL;
    struct MinHeapNode* root = heap->array[0];
    struct MinHeapNode* last_node = heap->array[heap->size - 1];
    heap->array[0] = last_node;
    heap->pos[root->vertex] = heap->size - 1;
    heap->pos[last_node->vertex] = 0;
    heap->size--;
    min_heapify(heap, 0);
    return root;
}

void decrease_key(struct MinHeap* heap, int vertex, int distance) {
    if (!heap || vertex < 0 || vertex >= heap->capacity) return;
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
    if (!heap || vertex < 0 || vertex >= heap->capacity) return 0;
    return heap->pos[vertex] < heap->size;
}

void dijkstra(struct Graph* graph, int src) {
    if (!graph || src < 0 || src >= graph->num_vertices) return;
    int V = graph->num_vertices;
    int* dist = malloc(V * sizeof(int));
    if (!dist) return;
    struct MinHeap* heap = create_min_heap(V);
    if (!heap) {
        free(dist);
        return;
    }
    for (int v = 0; v < V; v++) {
        dist[v] = INT_MAX;
        heap->array[v] = new_min_heap_node(v, dist[v]);
        if (!heap->array[v]) {
            for (int j = 0; j < v; j++) free(heap->array[j]);
            free(heap->array);
            free(heap->pos);
            free(heap);
            free(dist);
            return;
        }
        heap->pos[v] = v;
    }
    heap->size = V;
    dist[src