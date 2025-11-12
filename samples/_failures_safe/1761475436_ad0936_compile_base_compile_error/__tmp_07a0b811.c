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

struct StackNode {
    int data;
    struct StackNode* next;
};

struct Stack {
    struct StackNode* top;
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
        heap->pos[heap->array[smallest]->v] = idx;
        heap->pos[heap->array[idx]->v] = smallest;
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

struct StackNode* newStackNode(int data) {
    struct StackNode* node = (struct StackNode*)malloc(sizeof(struct StackNode));
    if (node == NULL) exit(1);
    node->data = data;
    node->next = NULL;
    return node;
}

struct Stack* createStack() {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    if (stack == NULL) exit(1);
    stack->top = NULL;
    return stack;
}

void push(struct Stack* stack, int data) {
    struct StackNode* node = newStackNode(data);
    node->next = stack->top;
    stack->top = node;
}

int pop(struct Stack* stack) {
    if (stack->top == NULL) return -1;
    struct StackNode* temp = stack->top;
    stack->top = stack->top->next;
    int popped = temp->data;
    free(temp);
    return popped;
}

int isEmptyStack(struct Stack* stack) {
    return stack->top == NULL;
}

void dijkstra(struct Graph* graph, int src, int dist[]) {
    int V = graph->V;
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
        struct