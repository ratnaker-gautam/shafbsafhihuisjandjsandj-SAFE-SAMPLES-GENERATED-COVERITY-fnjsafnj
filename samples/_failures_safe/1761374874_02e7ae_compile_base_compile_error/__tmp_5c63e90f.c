//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    uint32_t count;
} LogEntry;

typedef struct {
    LogEntry *entries;
    size_t size;
    size_t capacity;
} LogHeap;

void init_log_heap(LogHeap *heap, size_t capacity) {
    if (capacity == 0 || capacity > MAX_ENTRIES) capacity = MAX_ENTRIES;
    heap->entries = malloc(capacity * sizeof(LogEntry));
    if (heap->entries == NULL) exit(EXIT_FAILURE);
    heap->size = 0;
    heap->capacity = capacity;
}

void free_log_heap(LogHeap *heap) {
    if (heap->entries != NULL) free(heap->entries);
    heap->entries = NULL;
    heap->size = 0;
    heap->capacity = 0;
}

int add_log_entry(LogHeap *heap, const char *timestamp, const char *level, const char *message) {
    if (heap == NULL || timestamp == NULL || level == NULL || message == NULL) return 0;
    if (heap->size >= heap->capacity) return 0;
    
    LogEntry *entry = &heap->entries[heap->size];
    
    size_t ts_len = strlen(timestamp);
    if (ts_len >= sizeof(entry->timestamp)) return 0;
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    size_t level_len = strlen(level);
    if (level_len >= sizeof(entry->level)) return 0;
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    size_t msg_len = strlen(message);
    if (msg_len >= sizeof(entry->message)) return 0;
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    entry->count = 1;
    heap->size++;
    return 1;
}

void analyze_log_levels(LogHeap *heap) {
    if (heap == NULL || heap->size == 0) return;
    
    char levels[5][16] = {"INFO", "WARN", "ERROR", "DEBUG", "FATAL"};
    uint32_t counts[5] = {0};
    
    for (size_t i = 0; i < heap->size; i++) {
        for (int j = 0; j < 5; j++) {
            if (strcmp(heap->entries[i].level, levels[j]) == 0) {
                counts[j]++;
                break;
            }
        }
    }
    
    printf("Log Level Analysis:\n");
    for (int i = 0; i < 5; i++) {
        printf("%s: %u\n", levels[i], counts[i]);
    }
}

void process_log_data(void) {
    LogHeap heap;
    init_log_heap(&heap, 50);
    
    LogEntry stack_logs[10];
    size_t stack_count = 0;
    
    char line[MAX_LINE_LEN];
    char timestamp[32];
    char level[16];
    char message[256];
    
    printf("Enter log entries (format: timestamp level message):\n");
    printf("Type 'END' to finish input.\n");
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (result != 3) {
            printf("Invalid format. Use: timestamp level message\n");
            continue;
        }
        
        if (stack_count < 10) {
            LogEntry *entry = &stack_logs[stack_count];
            strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
            entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
            strncpy(entry->level, level, sizeof(entry->level) - 1);
            entry->level[sizeof(entry->level) - 1] = '\0';
            strncpy(entry->message, message, sizeof(entry->message) - 1);
            entry->message[sizeof(entry->message) - 1] = '\0';
            entry->count = 1;
            stack_count++;
        }
        
        if (!add_log_entry(&heap, timestamp, level, message)) {
            printf("Failed to add log entry (heap full)\n");
        }
    }
    
    printf("\nProcessing complete. %zu entries in heap, %zu in stack.\n", heap.size, stack_count);
    
    analyze_log_levels(&heap);
    
    printf("\nFirst 5 stack entries:\n");
    for (size_t i = 0; i < stack_count && i <