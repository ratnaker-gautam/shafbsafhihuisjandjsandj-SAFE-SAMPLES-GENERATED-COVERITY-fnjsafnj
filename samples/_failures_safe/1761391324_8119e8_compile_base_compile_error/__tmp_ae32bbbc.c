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
    int count;
    int capacity;
} LogHeap;

typedef struct {
    LogEntry entries[MAX_ENTRIES];
    int count;
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
}

int add_to_heap_log(LogHeap *heap, const char *timestamp, const char *level, const char *message) {
    if (heap->count >= heap->capacity) {
        int new_capacity = heap->capacity * 2;
        if (new_capacity > MAX_ENTRIES) {
            new_capacity = MAX_ENTRIES;
        }
        LogEntry *new_entries = realloc(heap->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) {
            return 0;
        }
        heap->entries = new_entries;
        heap->capacity = new_capacity;
    }
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) {
        return 0;
    }
    
    strcpy(heap->entries[heap->count].timestamp, timestamp);
    strcpy(heap->entries[heap->count].level, level);
    strcpy(heap->entries[heap->count].message, message);
    heap->count++;
    return 1;
}

void init_stack_log(LogStack *stack) {
    stack->count = 0;
}

int add_to_stack_log(LogStack *stack, const char *timestamp, const char *level, const char *message) {
    if (stack->count >= MAX_ENTRIES) {
        return 0;
    }
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) {
        return 0;
    }
    
    strcpy(stack->entries[stack->count].timestamp, timestamp);
    strcpy(stack->entries[stack->count].level, level);
    strcpy(stack->entries[stack->count].message, message);
    stack->count++;
    return 1;
}

void generate_sample_logs(LogHeap *heap, LogStack *stack) {
    const char *levels[] = {"INFO", "WARNING", "ERROR"};
    const char *messages[] = {
        "System started",
        "User login successful",
        "Database connection established",
        "Low disk space",
        "Network timeout",
        "Application shutdown"
    };
    
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    
    for (int i = 0; i < 20; i++) {
        char timestamp[32];
        tm_info->tm_min = (tm_info->tm_min + i) % 60;
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
        
        const char *level = levels[i % 3];
        const char *message = messages[i % 6];
        
        if (i % 2 == 0) {
            add_to_heap_log(heap, timestamp, level, message);
        } else {
            add_to_stack_log(stack, timestamp, level, message);
        }
    }
}

void analyze_logs(LogHeap *heap, LogStack *stack) {
    int info_count = 0, warning_count = 0, error_count = 0;
    
    for (int i = 0; i < heap->count; i++) {
        if (strcmp(heap->entries[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(heap->entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(heap->entries[i].level, "ERROR") == 0) {
            error_count++;
        }
    }
    
    for (int i = 0; i < stack->count; i++) {
        if (strcmp(stack->entries[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(stack->entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(stack->entries[i].level, "ERROR") == 0) {
            error_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", heap->count + stack->count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    printf("Heap entries: %d\n", heap->count);
    printf("Stack entries: %d\n", stack->count);
}

void display_recent_logs(LogHeap *heap, LogStack *stack, int count) {
    if (count <= 0) {
        return;