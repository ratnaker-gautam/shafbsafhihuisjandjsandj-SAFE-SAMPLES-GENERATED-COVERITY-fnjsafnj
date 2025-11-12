//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

enum GraphType {
    ADJ_MATRIX,
    ADJ_LIST
};

enum Operation {
    ADD_EDGE,
    REMOVE_EDGE,
    FIND_PATH,
    CALC_DEGREE,
    EXIT
};

struct Graph {
    enum GraphType type;
    int vertex_count;
    int **adj_matrix;
    int *degrees;
};

int validate_vertex(struct Graph *g, int v) {
    return (v >= 0 && v < g->vertex_count);
}

int validate_weight(int w) {
    return (w >= 0 && w <= 1000);
}

void init_graph(struct Graph *g, enum GraphType t, int vertices) {
    if (vertices <= 0 || vertices > 100) {
        vertices = 5;
    }
    
    g->type = t;
    g->vertex_count = vertices;
    
    if (t == ADJ_MATRIX) {
        g->adj_matrix = malloc(vertices * sizeof(int *));
        for (int i = 0; i < vertices; i++) {
            g->adj_matrix[i] = calloc(vertices, sizeof(int));
        }
        g->degrees = NULL;
    } else {
        g->adj_matrix = NULL;
        g->degrees = calloc(vertices, sizeof(int));
    }
}

void free_graph(struct Graph *g) {
    if (g->type == ADJ_MATRIX && g->adj_matrix) {
        for (int i = 0; i < g->vertex_count; i++) {
            free(g->adj_matrix[i]);
        }
        free(g->adj_matrix);
    } else if (g->degrees) {
        free(g->degrees);
    }
}

int add_edge_matrix(struct Graph *g, int src, int dest, int weight) {
    if (!validate_vertex(g, src) || !validate_vertex(g, dest) || !validate_weight(weight)) {
        return 0;
    }
    g->adj_matrix[src][dest] = weight;
    g->adj_matrix[dest][src] = weight;
    return 1;
}

int remove_edge_matrix(struct Graph *g, int src, int dest) {
    if (!validate_vertex(g, src) || !validate_vertex(g, dest)) {
        return 0;
    }
    g->adj_matrix[src][dest] = 0;
    g->adj_matrix[dest][src] = 0;
    return 1;
}

int find_path_matrix(struct Graph *g, int start, int end) {
    if (!validate_vertex(g, start) || !validate_vertex(g, end)) {
        return 0;
    }
    
    int *visited = calloc(g->vertex_count, sizeof(int));
    int *stack = malloc(g->vertex_count * sizeof(int));
    int stack_top = -1;
    
    if (!visited || !stack) {
        if (visited) free(visited);
        if (stack) free(stack);
        return 0;
    }
    
    stack[++stack_top] = start;
    visited[start] = 1;
    
    while (stack_top >= 0) {
        int current = stack[stack_top--];
        
        if (current == end) {
            free(visited);
            free(stack);
            return 1;
        }
        
        for (int i = 0; i < g->vertex_count; i++) {
            if (g->adj_matrix[current][i] > 0 && !visited[i]) {
                visited[i] = 1;
                stack[++stack_top] = i;
            }
        }
    }
    
    free(visited);
    free(stack);
    return 0;
}

int calculate_degree_matrix(struct Graph *g, int vertex) {
    if (!validate_vertex(g, vertex)) {
        return -1;
    }
    
    int degree = 0;
    for (int i = 0; i < g->vertex_count; i++) {
        if (g->adj_matrix[vertex][i] > 0) {
            degree++;
        }
    }
    return degree;
}

void handle_add_edge(struct Graph *g) {
    int src, dest, weight;
    printf("Enter source, destination, weight: ");
    if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        return;
    }
    
    int result;
    switch (g->type) {
        case ADJ_MATRIX:
            result = add_edge_matrix(g, src, dest, weight);
            break;
        default:
            result = 0;
            break;
    }
    
    printf(result ? "Edge added\n" : "Failed to add edge\n");
}

void handle_remove_edge(struct Graph *g) {
    int src, dest;
    printf("Enter source, destination: ");
    if (scanf("%d %d", &src, &dest) != 2) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        return;
    }
    
    int result;
    switch (g->type) {
        case ADJ_MATRIX:
            result = remove_edge_matrix(g, src, dest);
            break;
        default:
            result = 0;
            break;
    }
    
    printf(result ? "Edge removed\n" : "Failed to remove edge\n");
}

void handle_find_path(struct Graph *g) {
    int start, end;
    printf("