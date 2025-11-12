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
    int numNodes;
    int numEdges;
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

struct MinHeapNode* newMinHeapNode(int vertex, int distance) {
    struct MinHeapNode* node = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    if (node == NULL) return NULL;
    node->vertex = vertex;
    node->distance = distance;
    return node;
}

struct MinHeap* createMinHeap(int capacity) {
    if (capacity <= 0 || capacity > MAX_NODES) return NULL;
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
    if (heap == NULL || idx < 0 || idx >= heap->size) return;
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
        swapMinHeapNode(&heap->array[smallest], &heap->array[idx]);
        minHeapify(heap, smallest);
    }
}

int isEmpty(struct MinHeap* heap) {
    if (heap == NULL) return 1;
    return heap->size == 0;
}

struct MinHeapNode* extractMin(struct MinHeap* heap) {
    if (heap == NULL || heap->size <= 0) return NULL;
    struct MinHeapNode* root = heap->array[0];
    struct MinHeapNode* lastNode = heap->array[heap->size - 1];
    heap->array[0] = lastNode;
    heap->pos[root->vertex] = heap->size - 1;
    heap->pos[lastNode->vertex] = 0;
    heap->size--;
    minHeapify(heap, 0);
    return root;
}

void decreaseKey(struct MinHeap* heap, int vertex, int distance) {
    if (heap == NULL || vertex < 0 || vertex >= heap->capacity) return;
    int i = heap->pos[vertex];
    heap->array[i]->distance = distance;
    while (i && heap->array[i]->distance < heap->array[(i - 1) / 2]->distance) {
        heap->pos[heap->array[i]->vertex] = (i - 1) / 2;
        heap->pos[heap->array[(i - 1) / 2]->vertex] = i;
        swapMinHeapNode(&heap->array[i], &heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int isInMinHeap(struct MinHeap* heap, int vertex) {
    if (heap == NULL || vertex < 0 || vertex >= heap->capacity) return 0;
    return heap->pos[vertex] < heap->size;
}

void dijkstra(struct Graph* graph, int src) {
    if (graph == NULL || src < 0 || src >= graph->numNodes) return;
    int V = graph->numNodes;
    int dist[V];
    struct MinHeap* heap = createMinHeap(V);
    if (heap == NULL) return;
    for (int v = 0; v < V; v++) {
        dist[v] = INT_MAX;
        heap->array[v] = newMinHeapNode(v, dist[v]);
        if (heap->array[v] == NULL) {
            for (int j = 0; j < v; j++) free(heap->array[j]);
            free(heap->array);
            free(heap->pos);
            free(heap);
            return;
        }
        heap->pos[v] = v;
    }
    heap->array[src] = newMinHeapNode(src, dist[src]);
    if (heap->array[src] == NULL) {
        for (int j = 0; j < V; j++) free(