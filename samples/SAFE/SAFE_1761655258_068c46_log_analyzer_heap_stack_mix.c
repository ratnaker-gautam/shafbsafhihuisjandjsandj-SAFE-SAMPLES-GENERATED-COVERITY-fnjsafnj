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
    if (capacity == 0 || capacity > 100000) capacity = 100;
    heap->entries = malloc(capacity * sizeof(LogEntry));
    if (heap->entries == NULL) exit(1);
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
    if (heap->size >= heap->capacity) return 0;
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    LogEntry *entry = &heap->entries[heap->size];
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp)-1);
    entry->timestamp[sizeof(entry->timestamp)-1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level)-1);
    entry->level[sizeof(entry->level)-1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message)-1);
    entry->message[sizeof(entry->message)-1] = '\0';
    entry->count = 1;
    
    heap->size++;
    return 1;
}

void analyze_logs_stack(LogHeap *heap) {
    if (heap->size == 0) return;
    
    uint32_t level_counts[4] = {0};
    char levels[4][16] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    
    for (size_t i = 0; i < heap->size; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(heap->entries[i].level, levels[j]) == 0) {
                level_counts[j] += heap->entries[i].count;
                break;
            }
        }
    }
    
    printf("Log Level Summary:\n");
    for (int i = 0; i < 4; i++) {
        printf("%s: %u\n", levels[i], level_counts[i]);
    }
}

int read_log_file(const char *filename, LogHeap *heap) {
    if (filename == NULL || heap == NULL) return 0;
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LEN];
    char timestamp[32];
    char level[16];
    char message[256];
    int line_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && line_count < 100) {
        line_count++;
        if (strlen(line) < 10) continue;
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed == 3) {
            if (!add_log_entry(heap, timestamp, level, message)) break;
        }
    }
    
    fclose(file);
    return line_count > 0;
}

int main(void) {
    LogHeap heap;
    init_log_heap(&heap, 50);
    
    char filename[256];
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        free_log_heap(&heap);
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len-1] == '\n') filename[len-1] = '\0';
    
    if (strlen(filename) == 0) {
        printf("No filename provided.\n");
        free_log_heap(&heap);
        return 1;
    }
    
    if (!read_log_file(filename, &heap)) {
        printf("Could not read log file: %s\n", filename);
        free_log_heap(&heap);
        return 1;
    }
    
    if (heap.size == 0) {
        printf("No valid log entries found.\n");
        free_log_heap(&heap);
        return 1;
    }
    
    printf("Successfully loaded %zu log entries.\n", heap.size);
    analyze_logs_stack(&heap);
    
    free_log_heap(&heap);
    return 0;
}