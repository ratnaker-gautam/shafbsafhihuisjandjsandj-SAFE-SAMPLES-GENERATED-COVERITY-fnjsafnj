//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

typedef struct {
    int u, v, weight;
} Edge;

typedef struct {
    Edge edges[MAX_EDGES];
    int edge_count;
    int node_count;
} Graph;

typedef struct {
    int node;
    int distance;
} HeapNode;

typedef struct {
    HeapNode nodes[MAX_NODES];
    int size;
} MinHeap;

void heap_init(MinHeap *heap) {
    heap->size = 0;
}

void heap_swap(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heap_push(MinHeap *heap, int node, int distance) {
    if (heap->size >= MAX_NODES) return;
    
    int i = heap->size;
    heap->nodes[i].node = node;
    heap->nodes[i].distance = distance;
    heap->size++;
    
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap->nodes[parent].distance <= heap->nodes[i].distance) break;
        heap_swap(&heap->nodes[parent], &heap->nodes[i]);
        i = parent;
    }
}

int heap_pop(MinHeap *heap) {
    if (heap->size <= 0) return -1;
    
    int min_node = heap->nodes[0].node;
    heap->size--;
    heap->nodes[0] = heap->nodes[heap->size];
    
    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        
        if (left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance) {
            smallest = left;
        }
        if (right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance) {
            smallest = right;
        }
        if (smallest == i) break;
        
        heap_swap(&heap->nodes[i], &heap->nodes[smallest]);
        i = smallest;
    }
    
    return min_node;
}

void graph_init(Graph *graph, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) node_count = MAX_NODES;
    graph->node_count = node_count;
    graph->edge_count = 0;
}

int graph_add_edge(Graph *graph, int u, int v, int weight) {
    if (u < 0 || u >= graph->node_count || v < 0 || v >= graph->node_count) return 0;
    if (graph->edge_count >= MAX_EDGES) return 0;
    if (weight < 0) return 0;
    
    graph->edges[graph->edge_count].u = u;
    graph->edges[graph->edge_count].v = v;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void dijkstra(Graph *graph, int start, int distances[]) {
    if (start < 0 || start >= graph->node_count) return;
    
    int visited[MAX_NODES];
    for (int i = 0; i < graph->node_count; i++) {
        distances[i] = INT_MAX;
        visited[i] = 0;
    }
    distances[start] = 0;
    
    MinHeap heap;
    heap_init(&heap);
    heap_push(&heap, start, 0);
    
    while (heap.size > 0) {
        int current = heap_pop(&heap);
        if (current == -1) break;
        if (visited[current]) continue;
        visited[current] = 1;
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].u == current) {
                int neighbor = graph->edges[i].v;
                int new_dist = distances[current] + graph->edges[i].weight;
                if (new_dist >= 0 && new_dist < distances[neighbor]) {
                    distances[neighbor] = new_dist;
                    heap_push(&heap, neighbor, new_dist);
                }
            }
        }
    }
}

int main() {
    Graph graph;
    int distances[MAX_NODES];
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    int node_count;
    if (scanf("%d", &node_count) != 1 || node_count <= 0 || node_count > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    graph_init(&graph, node_count);
    
    printf("Enter number of edges: ");
    int edge_count;
    if (scanf("%d", &edge_count) != 1 || edge_count < 0 || edge_count > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'u v weight' (0-indexed):\n");
    for (int i = 0; i < edge_count; i++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight)