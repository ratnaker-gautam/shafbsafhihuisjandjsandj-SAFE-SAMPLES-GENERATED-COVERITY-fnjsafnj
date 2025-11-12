//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
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
    int v;
    int dist;
};

struct MinHeap {
    int size;
    int capacity;
    int *pos;
    struct MinHeapNode **array;
};

struct MinHeapNode* newMinHeapNode(int v, int dist) {
    struct MinHeapNode* minHeapNode = (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    if (minHeapNode == NULL) return NULL;
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

struct MinHeap* createMinHeap(int capacity) {
    if (capacity <= 0 || capacity > MAX_NODES) return NULL;
    struct MinHeap* minHeap = (struct MinHeap*) malloc(sizeof(struct MinHeap));
    if (minHeap == NULL) return NULL;
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    if (minHeap->pos == NULL) {
        free(minHeap);
        return NULL;
    }
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    if (minHeap->array == NULL) {
        free(minHeap->pos);
        free(minHeap);
        return NULL;
    }
    return minHeap;
}

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx) {
    if (minHeap == NULL || idx < 0 || idx >= minHeap->size) return;
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist)
        smallest = right;

    if (smallest != idx) {
        struct MinHeapNode *smallestNode = minHeap->array[smallest];
        struct MinHeapNode *idxNode = minHeap->array[idx];

        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isEmpty(struct MinHeap* minHeap) {
    return minHeap == NULL || minHeap->size == 0;
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    if (isEmpty(minHeap))
        return NULL;

    struct MinHeapNode* root = minHeap->array[0];
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    minHeap->pos[root->v] = minHeap->size - 1;
    minHeap->pos[lastNode->v] = 0;

    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

void decreaseKey(struct MinHeap* minHeap, int v, int dist) {
    if (minHeap == NULL || v < 0 || v >= minHeap->capacity) return;
    int i = minHeap->pos[v];
    if (i >= minHeap->size) return;
    minHeap->array[i]->dist = dist;

    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) {
        minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int isInMinHeap(struct MinHeap *minHeap, int v) {
    if (minHeap == NULL || v < 0 || v >= minHeap->capacity) return 0;
    if (minHeap->pos[v] < minHeap->size)
        return 1;
    return 0;
}

void dijkstra(struct Graph* graph, int src) {
    if (graph == NULL || src < 0 || src >= graph->V || graph->V > MAX_NODES) return;
    int V = graph->V;
    int dist[MAX_NODES];
    struct MinHeap* minHeap = createMinHeap(V);
    if (minHeap == NULL) return;

    for (int v = 0; v < V; ++v) {
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        if (minHeap->array[v] == NULL) {
            for (int j = 0; j <