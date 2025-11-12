//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

typedef void (*GraphOperation)(Graph*, int, int);

void initialize_graph(Graph* graph, int node_count) {
    if (graph == NULL || node_count <= 0 || node_count > MAX_NODES) {
        return;
    }
    graph->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            graph->adjacency[i][j] = 0;
        }
    }
}

void add_edge(Graph* graph, int from, int to) {
    if (graph == NULL || from < 0 || from >= graph->node_count || 
        to < 0 || to >= graph->node_count) {
        return;
    }
    graph->adjacency[from][to] = 1;
    graph->adjacency[to][from] = 1;
}

void depth_first_traversal(Graph* graph, int start, int visited[]) {
    if (graph == NULL || start < 0 || start >= graph->node_count || visited == NULL) {
        return;
    }
    visited[start] = 1;
    printf("%d ", start);
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->adjacency[start][i] && !visited[i]) {
            depth_first_traversal(graph, i, visited);
        }
    }
}

void breadth_first_traversal(Graph* graph, int start, int visited[]) {
    if (graph == NULL || start < 0 || start >= graph->node_count || visited == NULL) {
        return;
    }
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        for (int i = 0; i < graph->node_count; i++) {
            if (graph->adjacency[current][i] && !visited[i]) {
                visited[i] = 1;
                queue[rear++] = i;
            }
        }
    }
}

void perform_operation(Graph* graph, int start, GraphOperation operation) {
    if (graph == NULL || start < 0 || start >= graph->node_count || operation == NULL) {
        return;
    }
    int visited[MAX_NODES] = {0};
    operation(graph, start, visited);
    printf("\n");
}

int get_valid_input(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
    }
}

int main() {
    Graph graph;
    int node_count = get_valid_input("Enter number of nodes (1-100): ", 1, MAX_NODES);
    initialize_graph(&graph, node_count);
    
    int edge_count = get_valid_input("Enter number of edges: ", 0, node_count * (node_count - 1) / 2);
    
    for (int i = 0; i < edge_count; i++) {
        printf("Edge %d:\n", i + 1);
        int from = get_valid_input("  From node: ", 0, node_count - 1);
        int to = get_valid_input("  To node: ", 0, node_count - 1);
        add_edge(&graph, from, to);
    }
    
    GraphOperation operations[] = {depth_first_traversal, breadth_first_traversal};
    const char* operation_names[] = {"Depth-First Search", "Breadth-First Search"};
    int operation_count = 2;
    
    int start_node = get_valid_input("Enter starting node for traversal: ", 0, node_count - 1);
    
    printf("\nGraph Traversal Results:\n");
    for (int i = 0; i < operation_count; i++) {
        printf("%s: ", operation_names[i]);
        perform_operation(&graph, start_node, operations[i]);
    }
    
    return 0;
}