//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    int key;
};

struct MinHeap {
    int size;
    int capacity;
    int *pos;
    struct MinHeapNode **array;
};

struct MinHeapNode* newMinHeapNode(int vertex, int key) {
    struct MinHeapNode* node = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    if (node == NULL) return NULL;
    node->vertex = vertex;
    node->key = key;
    return node;
}

struct MinHeap* createMinHeap(int capacity) {
    struct MinHeap* heap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    if (heap == NULL) return NULL;
    heap->pos = (int*)malloc(capacity * sizeof(int));
    if (heap->pos == NULL) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    heap->array = (struct MinHeapNode**)malloc(capacity * sizeof(struct MinHeapNode*));
    if (heap->array == NULL) {
        free(heap->pos);
        free(heap);
        return NULL;
    }
    return heap;
}

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(struct MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < heap->size && heap->array[left]->key < heap->array[smallest]->key) {
        smallest = left;
    }
    if (right < heap->size && heap->array[right]->key < heap->array[smallest]->key) {
        smallest = right;
    }
    if (smallest != idx) {
        struct MinHeapNode* smallestNode = heap->array[smallest];
        struct MinHeapNode* idxNode = heap->array[idx];
        heap->pos[smallestNode->vertex] = idx;
        heap->pos[idxNode->vertex] = smallest;
        swapMinHeapNode(&heap->array[smallest], &heap->array[idx]);
        minHeapify(heap, smallest);
    }
}

struct MinHeapNode* extractMin(struct MinHeap* heap) {
    if (heap->size == 0) return NULL;
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

int isInMinHeap(struct MinHeap* heap, int vertex) {
    if (vertex < 0 || vertex >= heap->capacity) return 0;
    return heap->pos[vertex] < heap->size;
}

void primMST(struct Graph* graph) {
    if (graph == NULL || graph->num_nodes <= 0 || graph->num_nodes > MAX_NODES) return;
    int num_nodes = graph->num_nodes;
    int parent[MAX_NODES];
    int key[MAX_NODES];
    struct MinHeap* heap = createMinHeap(num_nodes);
    if (heap == NULL) return;
    for (int v = 1; v < num_nodes; v++) {
        parent[v] = -1;
        key[v] = INT_MAX;
        heap->array[v] = newMinHeapNode(v, key[v]);
        if (heap->array[v] == NULL) {
            for (int j = 0; j < v; j++) free(heap->array[j]);
            free(heap->array);
            free(heap->pos);
            free(heap);
            return;
        }
        heap->pos[v] = v;
    }
    key[0] = 0;
    heap->array[0] = newMinHeapNode(0, key[0]);
    if (heap->array[0] == NULL) {
        for (int j = 1; j < num_nodes; j++) free(heap->array[j]);
        free(heap->array);
        free(heap->pos);
        free(heap);
        return;
    }
    heap->pos[0] =