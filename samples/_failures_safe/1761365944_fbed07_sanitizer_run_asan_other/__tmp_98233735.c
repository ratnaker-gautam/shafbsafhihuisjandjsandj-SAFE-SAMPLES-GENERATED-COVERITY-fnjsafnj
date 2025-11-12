//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
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

int validate_node(Graph* graph, int node) {
    return graph != NULL && node >= 0 && node < graph->node_count;
}

void add_edge(Graph* graph, int from, int to) {
    if (!validate_node(graph, from) || !validate_node(graph, to)) {
        return;
    }
    graph->adjacency[from][to] = 1;
    graph->adjacency[to][from] = 1;
}

void depth_first_traversal(Graph* graph, int start, int visited[]) {
    if (graph == NULL || visited == NULL || !validate_node(graph, start)) {
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
    if (graph == NULL || visited == NULL || !validate_node(graph, start)) {
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
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
}

void reset_visited(int visited[], int size) {
    if (visited == NULL || size <= 0 || size > MAX_NODES) {
        return;
    }
    for (int i = 0; i < size; i++) {
        visited[i] = 0;
    }
}

void perform_operation(Graph* graph, GraphOperation operation, int start) {
    if (graph == NULL || operation == NULL || !validate_node(graph, start)) {
        return;
    }
    int visited[MAX_NODES];
    reset_visited(visited, graph->node_count);
    operation(graph, start, visited);
    printf("\n");
}

void print_menu() {
    printf("1. Depth First Traversal\n");
    printf("2. Breadth First Traversal\n");
    printf("3. Exit\n");
    printf("Enter choice: ");
}

int main() {
    Graph graph;
    initialize_graph(&graph, 6);
    add_edge(&graph, 0, 1);
    add_edge(&graph, 0, 2);
    add_edge(&graph, 1, 3);
    add_edge(&graph, 2, 4);
    add_edge(&graph, 3, 5);
    add_edge(&graph, 4, 5);
    
    GraphOperation operations[] = {
        depth_first_traversal,
        breadth_first_traversal
    };
    
    int choice;
    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        if (choice == 3) {
            break;
        }
        if (choice < 1 || choice > 2) {
            printf("Invalid choice\n");
            continue;
        }
        printf("Enter start node (0-5): ");
        int start;
        if (scanf("%d", &start) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        if (start < 0 || start >= graph.node_count) {
            printf("Invalid node\n");
            continue;
        }
        perform_operation(&graph, operations[choice - 1], start);
    }
    return 0;
}