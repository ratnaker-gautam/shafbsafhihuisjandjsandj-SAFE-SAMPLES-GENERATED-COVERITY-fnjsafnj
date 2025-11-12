//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Edge {
    int src, dest, weight;
};

struct Graph {
    int V, E;
    struct Edge edges[MAX_EDGES];
};

struct MinHeapNode {
    int vertex, key;
};

struct MinHeap {
    int size, capacity;
    int *pos;
    struct MinHeapNode **array;
};

struct MinHeapNode* newMinHeapNode(int vertex, int key) {
    struct MinHeapNode* node = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    if (node == NULL) exit(1);
    node->vertex = vertex;
    node->key = key;
    return node;
}

struct MinHeap* createMinHeap(int capacity) {
    struct MinHeap* heap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    if (heap == NULL) exit(1);
    heap->pos = (int*)malloc(capacity * sizeof(int));
    if (heap->pos == NULL) exit(1);
    heap->size = 0;
    heap->capacity = capacity;
    heap->array = (struct MinHeapNode**)malloc(capacity * sizeof(struct MinHeapNode*));
    if (heap->array == NULL) exit(1);
    return heap;
}

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < heap->size && heap->array[left]->key < heap->array[smallest]->key) smallest = left;
    if (right < heap->size && heap->array[right]->key < heap->array[smallest]->key) smallest = right;
    if (smallest != idx) {
        heap->pos[heap->array[smallest]->vertex] = idx;
        heap->pos[heap->array[idx]->vertex] = smallest;
        swapMinHeapNode(&heap->array[smallest], &heap->array[idx]);
        minHeapify(heap, smallest);
    }
}

int isEmpty(struct MinHeap* heap) {
    return heap->size == 0;
}

struct MinHeapNode* extractMin(struct MinHeap* heap) {
    if (isEmpty(heap)) return NULL;
    struct MinHeapNode* root = heap->array[0];
    struct MinHeapNode* lastNode = heap->array[heap->size - 1];
    heap->array[0] = lastNode;
    heap->pos[root->vertex] = heap->size - 1;
    heap->pos[lastNode->vertex] = 0;
    heap->size--;
    minHeapify(heap, 0);
    return root;
}

void decreaseKey(struct MinHeap* heap, int vertex, int key) {
    int i = heap->pos[vertex];
    heap->array[i]->key = key;
    while (i && heap->array[i]->key < heap->array[(i - 1) / 2]->key) {
        heap->pos[heap->array[i]->vertex] = (i - 1) / 2;
        heap->pos[heap->array[(i - 1) / 2]->vertex] = i;
        swapMinHeapNode(&heap->array[i], &heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int isInMinHeap(struct MinHeap* heap, int v) {
    if (v < 0 || v >= heap->capacity) return 0;
    return heap->pos[v] < heap->size;
}

void primMST(struct Graph* graph) {
    if (graph == NULL || graph->V <= 0 || graph->V > MAX_NODES) return;
    int V = graph->V;
    int parent[V];
    int key[V];
    struct MinHeap* heap = createMinHeap(V);
    for (int v = 1; v < V; v++) {
        parent[v] = -1;
        key[v] = INT_MAX;
        heap->array[v] = newMinHeapNode(v, key[v]);
        heap->pos[v] = v;
    }
    key[0] = 0;
    heap->array[0] = newMinHeapNode(0, key[0]);
    heap->pos[0] = 0;
    heap->size = V;
    while (!isEmpty(heap)) {
        struct MinHeapNode* minNode = extractMin(heap);
        if (minNode == NULL) break;
        int u = minNode->vertex;
        for (int i = 0; i < graph->E; i++) {
            if (graph->edges[i].src == u || graph->edges[i].dest == u) {
                int v = (graph->edges[i].src == u) ? graph->edges[i].dest : graph->edges[i].src;
                if (v >= 0 && v < V && isInMinHeap(heap, v) && graph->edges[i].weight < key[v]) {
                    key[v] = graph->edges[i].weight;
                    parent[v] = u;
                    decreaseKey(heap, v, key[v]);
                }
            }
        }
        free(minNode