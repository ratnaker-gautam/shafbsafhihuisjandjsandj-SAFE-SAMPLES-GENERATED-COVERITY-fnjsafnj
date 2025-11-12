//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

typedef struct {
    LogEntry *entries;
    size_t count;
    size_t capacity;
} LogHeap;

typedef struct {
    LogEntry entries[MAX_ENTRIES];
    size_t count;
} LogStack;

void init_heap_log(LogHeap *heap) {
    heap->capacity = 100;
    heap->count = 0;
    heap->entries = malloc(heap->capacity * sizeof(LogEntry));
    if (!heap->entries) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
}

void free_heap_log(LogHeap *heap) {
    free(heap->entries);
    heap->entries = NULL;
    heap->count = 0;
    heap->capacity = 0;
}

int add_to_heap(LogHeap *heap, const char *timestamp, const char *level, const char *message) {
    if (heap->count >= heap->capacity) {
        size_t new_capacity = heap->capacity * 2;
        if (new_capacity <= heap->capacity) {
            return 0;
        }
        LogEntry *new_entries = realloc(heap->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) {
            return 0;
        }
        heap->entries = new_entries;
        heap->capacity = new_capacity;
    }
    
    if (strlen(timestamp) >= sizeof(heap->entries[heap->count].timestamp) ||
        strlen(level) >= sizeof(heap->entries[heap->count].level) ||
        strlen(message) >= sizeof(heap->entries[heap->count].message)) {
        return 0;
    }
    
    strncpy(heap->entries[heap->count].timestamp, timestamp, sizeof(heap->entries[heap->count].timestamp) - 1);
    strncpy(heap->entries[heap->count].level, level, sizeof(heap->entries[heap->count].level) - 1);
    strncpy(heap->entries[heap->count].message, message, sizeof(heap->entries[heap->count].message) - 1);
    
    heap->entries[heap->count].timestamp[sizeof(heap->entries[heap->count].timestamp) - 1] = '\0';
    heap->entries[heap->count].level[sizeof(heap->entries[heap->count].level) - 1] = '\0';
    heap->entries[heap->count].message[sizeof(heap->entries[heap->count].message) - 1] = '\0';
    
    heap->count++;
    return 1;
}

int add_to_stack(LogStack *stack, const char *timestamp, const char *level, const char *message) {
    if (stack->count >= MAX_ENTRIES) {
        return 0;
    }
    
    if (strlen(timestamp) >= sizeof(stack->entries[stack->count].timestamp) ||
        strlen(level) >= sizeof(stack->entries[stack->count].level) ||
        strlen(message) >= sizeof(stack->entries[stack->count].message)) {
        return 0;
    }
    
    strncpy(stack->entries[stack->count].timestamp, timestamp, sizeof(stack->entries[stack->count].timestamp) - 1);
    strncpy(stack->entries[stack->count].level, level, sizeof(stack->entries[stack->count].level) - 1);
    strncpy(stack->entries[stack->count].message, message, sizeof(stack->entries[stack->count].message) - 1);
    
    stack->entries[stack->count].timestamp[sizeof(stack->entries[stack->count].timestamp) - 1] = '\0';
    stack->entries[stack->count].level[sizeof(stack->entries[stack->count].level) - 1] = '\0';
    stack->entries[stack->count].message[sizeof(stack->entries[stack->count].message) - 1] = '\0';
    
    stack->count++;
    return 1;
}

void analyze_logs(LogHeap *heap, LogStack *stack) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (size_t i = 0; i < heap->count; i++) {
        if (strcmp(heap->entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(heap->entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(heap->entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    for (size_t i = 0; i < stack->count; i++) {
        if (strcmp(stack->entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(stack->entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(stack->entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", heap->