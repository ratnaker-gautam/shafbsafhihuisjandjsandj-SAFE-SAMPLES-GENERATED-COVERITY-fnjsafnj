//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct GraphNode {
    int id;
    int visited;
    int distance;
    int predecessor;
};

struct GraphEdge {
    int source;
    int destination;
    int weight;
};

struct Graph {
    struct GraphNode nodes[MAX_NODES];
    struct GraphEdge edges[MAX_EDGES];
    int node_count;
    int edge_count;
};

struct MinHeapNode {
    int vertex;
    int distance;
};

struct MinHeap {
    struct MinHeapNode array[MAX_NODES];
    int size;
    int capacity;
};

void initialize_graph(struct Graph* graph) {
    graph->node_count = 0;
    graph->edge_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        graph->nodes[i].id = -1;
        graph->nodes[i].visited = 0;
        graph->nodes[i].distance = INT_MAX;
        graph->nodes[i].predecessor = -1;
    }
}

int add_node(struct Graph* graph, int id) {
    if (graph->node_count >= MAX_NODES || id < 0 || id >= MAX_NODES) {
        return -1;
    }
    if (graph->nodes[id].id != -1) {
        return -1;
    }
    graph->nodes[id].id = id;
    graph->nodes[id].visited = 0;
    graph->nodes[id].distance = INT_MAX;
    graph->nodes[id].predecessor = -1;
    graph->node_count++;
    return 0;
}

int add_edge(struct Graph* graph, int source, int destination, int weight) {
    if (graph->edge_count >= MAX_EDGES) {
        return -1;
    }
    if (source < 0 || source >= MAX_NODES || destination < 0 || destination >= MAX_NODES) {
        return -1;
    }
    if (graph->nodes[source].id == -1 || graph->nodes[destination].id == -1) {
        return -1;
    }
    if (weight < 0) {
        return -1;
    }
    graph->edges[graph->edge_count].source = source;
    graph->edges[graph->edge_count].destination = destination;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 0;
}

void swap_min_heap_nodes(struct MinHeapNode* a, struct MinHeapNode* b) {
    struct MinHeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void min_heapify(struct MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < heap->size && heap->array[left].distance < heap->array[smallest].distance) {
        smallest = left;
    }
    if (right < heap->size && heap->array[right].distance < heap->array[smallest].distance) {
        smallest = right;
    }
    if (smallest != idx) {
        swap_min_heap_nodes(&heap->array[idx], &heap->array[smallest]);
        min_heapify(heap, smallest);
    }
}

struct MinHeapNode extract_min(struct MinHeap* heap) {
    if (heap->size <= 0) {
        struct MinHeapNode empty = {-1, INT_MAX};
        return empty;
    }
    if (heap->size == 1) {
        heap->size--;
        return heap->array[0];
    }
    struct MinHeapNode root = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;
    min_heapify(heap, 0);
    return root;
}

void decrease_key(struct MinHeap* heap, int vertex, int distance) {
    int i = 0;
    for (i = 0; i < heap->size; i++) {
        if (heap->array[i].vertex == vertex) {
            break;
        }
    }
    if (i >= heap->size) {
        return;
    }
    heap->array[i].distance = distance;
    while (i != 0 && heap->array[(i - 1) / 2].distance > heap->array[i].distance) {
        swap_min_heap_nodes(&heap->array[i], &heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int is_in_min_heap(struct MinHeap* heap, int vertex) {
    for (int i = 0; i < heap->size; i++) {
        if (heap->array[i].vertex == vertex) {
            return 1;
        }
    }
    return 0;
}

void dijkstra(struct Graph* graph, int source) {
    if (source < 0 || source >= MAX_NODES || graph->nodes[source].id == -1) {
        return;
    }
    for (int i = 0; i < MAX_NODES; i++) {
        if (graph->nodes[i].id != -1) {
            graph->nodes[i].distance = INT_MAX;
            graph->nodes[i].predecessor = -1;
        }
    }
    graph->nodes[source].distance =