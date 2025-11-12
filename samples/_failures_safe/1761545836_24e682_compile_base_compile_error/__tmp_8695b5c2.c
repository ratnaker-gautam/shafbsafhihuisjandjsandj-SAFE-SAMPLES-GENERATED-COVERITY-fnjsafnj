//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int num_nodes;
    int num_edges;
    int adjacency[MAX_NODES][MAX_NODES];
    char node_names[MAX_NODES][32];
};

void initialize_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g->adjacency[i][j] = 0;
        }
        g->node_names[i][0] = '\0';
    }
}

int find_node_index(struct Graph *g, const char *name) {
    for (int i = 0; i < g->num_nodes; i++) {
        if (strcmp(g->node_names[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

int add_node(struct Graph *g, const char *name) {
    if (g->num_nodes >= MAX_NODES) {
        return -1;
    }
    if (strlen(name) >= 32) {
        return -1;
    }
    if (find_node_index(g, name) != -1) {
        return -1;
    }
    strcpy(g->node_names[g->num_nodes], name);
    g->num_nodes++;
    return g->num_nodes - 1;
}

int add_edge(struct Graph *g, const char *from, const char *to, int weight) {
    if (g->num_edges >= MAX_EDGES) {
        return 0;
    }
    int from_idx = find_node_index(g, from);
    int to_idx = find_node_index(g, to);
    if (from_idx == -1 || to_idx == -1) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    g->adjacency[from_idx][to_idx] = weight;
    g->num_edges++;
    return 1;
}

void depth_first_search(struct Graph *g, int node, int visited[]) {
    if (node < 0 || node >= g->num_nodes) {
        return;
    }
    visited[node] = 1;
    printf("%s ", g->node_names[node]);
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adjacency[node][i] > 0 && !visited[i]) {
            depth_first_search(g, i, visited);
        }
    }
}

void perform_dfs(struct Graph *g, const char *start_node) {
    int visited[MAX_NODES] = {0};
    int start_idx = find_node_index(g, start_node);
    if (start_idx == -1) {
        printf("Start node not found.\n");
        return;
    }
    printf("DFS traversal from %s: ", start_node);
    depth_first_search(g, start_idx, visited);
    printf("\n");
}

void shortest_path_dijkstra(struct Graph *g, const char *start_node) {
    int start_idx = find_node_index(g, start_node);
    if (start_idx == -1) {
        printf("Start node not found.\n");
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = -1;
    }
    dist[start_idx] = 0;
    
    for (int count = 0; count < g->num_nodes - 1; count++) {
        int min_dist = -1;
        int min_node = -1;
        
        for (int i = 0; i < g->num_nodes; i++) {
            if (!visited[i] && dist[i] != -1 && (min_dist == -1 || dist[i] < min_dist)) {
                min_dist = dist[i];
                min_node = i;
            }
        }
        
        if (min_node == -1) {
            break;
        }
        
        visited[min_node] = 1;
        
        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adjacency[min_node][i] > 0 && !visited[i]) {
                int new_dist = dist[min_node] + g->adjacency[min_node][i];
                if (dist[i] == -1 || new_dist < dist[i]) {
                    dist[i] = new_dist;
                }
            }
        }
    }
    
    printf("Shortest paths from %s:\n", start_node);
    for (int i = 0; i < g->num_nodes; i++) {
        if (i != start_idx) {
            printf("  to %s: ", g->node_names[i]);
            if (dist[i] == -1) {
                printf("No path\n");
            } else {
                printf("%d\n", dist[i]);
            }
        }
    }
}

int main() {
    struct Graph g;
    initialize_graph(&g);
    
    printf("Graph Algorithms Demo\n");
    printf("Adding nodes...\n");
    
    char *nodes[] = {"A", "B", "C", "D", "E", "F"};
    int num_nodes = sizeof(nodes) / sizeof(nodes[0]);
    
    for (