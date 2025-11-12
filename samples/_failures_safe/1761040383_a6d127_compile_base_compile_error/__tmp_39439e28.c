//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
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
    if (heap->entries == NULL) {
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

void add_to_heap(LogHeap *heap, const char *timestamp, const char *level, const char *message) {
    if (heap->count >= heap->capacity) {
        int new_capacity = heap->capacity * 2;
        if (new_capacity > MAX_ENTRIES) {
            new_capacity = MAX_ENTRIES;
        }
        LogEntry *new_entries = realloc(heap->entries, new_capacity * sizeof(LogEntry));
        if (new_entries == NULL) {
            fprintf(stderr, "Memory reallocation failed\n");
            return;
        }
        heap->entries = new_entries;
        heap->capacity = new_capacity;
    }
    
    if (heap->count < heap->capacity) {
        LogEntry *entry = &heap->entries[heap->count];
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        heap->count++;
    }
}

void init_stack_log(LogStack *stack) {
    stack->count = 0;
}

void add_to_stack(LogStack *stack, const char *timestamp, const char *level, const char *message) {
    if (stack->count < MAX_ENTRIES) {
        LogEntry *entry = &stack->entries[stack->count];
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        stack->count++;
    }
}

void generate_sample_logs(LogHeap *heap, LogStack *stack) {
    const char *levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    const char *messages[] = {
        "User login successful",
        "Database connection established",
        "File not found",
        "Memory allocation failed",
        "Network timeout",
        "Configuration loaded",
        "Invalid input detected",
        "Backup completed"
    };
    
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    
    for (int i = 0; i < 50; i++) {
        char timestamp[32];
        int offset = rand() % 3600;
        time_t log_time = now - offset;
        struct tm *log_tm = localtime(&log_time);
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", log_tm);
        
        const char *level = levels[rand() % 4];
        const char *message = messages[rand() % 8];
        
        if (i % 2 == 0) {
            add_to_heap(heap, timestamp, level, message);
        } else {
            add_to_stack(stack, timestamp, level, message);
        }
    }
}

void analyze_logs(LogHeap *heap, LogStack *stack) {
    int total_logs = heap->count + stack->count;
    printf("Total logs analyzed: %d\n", total_logs);
    printf("Heap logs: %d\n", heap->count);
    printf("Stack logs: %d\n", stack->count);
    
    int level_counts[4] = {0};
    const char *levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    
    for (int i = 0; i < heap->count; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(heap->entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    for (int i = 0; i < stack->count; i++)