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
    int v, dist;
};

struct MinHeap {
    int size, capacity;
    int *pos;
    struct MinHeapNode **array;
};

struct MinHeapNode* newMinHeapNode(int v, int dist) {
    struct MinHeapNode* node = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    if (node == NULL) exit(1);
    node->v = v;
    node->dist = dist;
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
    if (left < heap->size && heap->array[left]->dist < heap->array[smallest]->dist) smallest = left;
    if (right < heap->size && heap->array[right]->dist < heap->array[smallest]->dist) smallest = right;
    if (smallest != idx) {
        struct MinHeapNode* smallestNode = heap->array[smallest];
        struct MinHeapNode* idxNode = heap->array[idx];
        heap->pos[smallestNode->v] = idx;
        heap->pos[idxNode->v] = smallest;
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
    heap->pos[root->v] = heap->size - 1;
    heap->pos[lastNode->v] = 0;
    heap->size--;
    minHeapify(heap, 0);
    return root;
}

void decreaseKey(struct MinHeap* heap, int v, int dist) {
    int i = heap->pos[v];
    heap->array[i]->dist = dist;
    while (i && heap->array[i]->dist < heap->array[(i - 1) / 2]->dist) {
        heap->pos[heap->array[i]->v] = (i - 1) / 2;
        heap->pos[heap->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&heap->array[i], &heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int isInMinHeap(struct MinHeap* heap, int v) {
    if (v >= heap->capacity) return 0;
    return heap->pos[v] < heap->size;
}

void dijkstra(struct Graph* graph, int src) {
    if (graph == NULL || src < 0 || src >= graph->V) return;
    
    int V = graph->V;
    int dist[MAX_NODES];
    struct MinHeap* heap = createMinHeap(V);
    for (int v = 0; v < V; v++) {
        dist[v] = INT_MAX;
        heap->array[v] = newMinHeapNode(v, dist[v]);
        heap->pos[v] = v;
    }
    heap->array[src] = newMinHeapNode(src, dist[src]);
    heap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(heap, src, dist[src]);
    heap->size = V;
    while (!isEmpty(heap)) {
        struct MinHeapNode* node = extractMin(heap);
        int u = node->v;
        for (int i = 0; i < graph->E; i++) {
            if (graph->edges[i].src == u) {
                int v = graph->edges[i].dest;
                int weight = graph->edges[i].weight;
                if (isInMinHeap(heap, v) && dist[u] != INT_MAX && weight + dist[u] < dist[v]) {
                    dist[v] = dist[u] + weight;
                    decreaseKey(heap, v, dist[v]);
                }
            }
        }
        free(node);
    }
    printf("Vertex Distance from Source\n");
    for (int i = 0; i < V;