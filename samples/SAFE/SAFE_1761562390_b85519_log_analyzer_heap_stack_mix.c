//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

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
    size_t count;
    size_t capacity;
} LogHeap;

int parse_log_line(const char *line, LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->count = 1;
    
    return 1;
}

int add_log_entry(LogHeap *heap, const LogEntry *entry) {
    if (!heap || !entry) return 0;
    
    if (heap->count >= heap->capacity) {
        size_t new_capacity = heap->capacity * 2;
        if (new_capacity < heap->capacity) return 0;
        
        LogEntry *new_entries = realloc(heap->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return 0;
        
        heap->entries = new_entries;
        heap->capacity = new_capacity;
    }
    
    heap->entries[heap->count] = *entry;
    heap->count++;
    return 1;
}

void process_log_stack(LogHeap *heap) {
    if (!heap || heap->count == 0) return;
    
    LogEntry stack[MAX_ENTRIES];
    size_t stack_count = 0;
    
    for (size_t i = 0; i < heap->count; i++) {
        int found = 0;
        for (size_t j = 0; j < stack_count; j++) {
            if (strcmp(heap->entries[i].level, stack[j].level) == 0) {
                stack[j].count += heap->entries[i].count;
                found = 1;
                break;
            }
        }
        
        if (!found && stack_count < MAX_ENTRIES) {
            stack[stack_count] = heap->entries[i];
            stack_count++;
        }
    }
    
    printf("Log Level Summary:\n");
    for (size_t i = 0; i < stack_count; i++) {
        printf("%s: %u entries\n", stack[i].level, stack[i].count);
    }
}

int main(void) {
    LogHeap heap = {0};
    heap.capacity = 10;
    heap.entries = malloc(heap.capacity * sizeof(LogEntry));
    if (!heap.entries) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char input_buffer[MAX_LINE_LEN];
    printf("Enter log lines (empty line to finish):\n");
    
    while (fgets(input_buffer, sizeof(input_buffer), stdin)) {
        if (input_buffer[0] == '\n') break;
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len-1] == '\n') {
            input_buffer[len-1] = '\0';
        }
        
        LogEntry entry;
        if (parse_log_line(input_buffer, &entry)) {
            if (!add_log_entry(&heap, &entry)) {
                fprintf(stderr, "Failed to add log entry\n");
                break;
            }
        } else {
            fprintf(stderr, "Failed to parse log line: %s\n", input_buffer);
        }
    }
    
    if (heap.count > 0) {
        process_log_stack(&heap);
        printf("Total log entries processed: %zu\n", heap.count);
    } else {
        printf("No valid log entries processed\n");
    }
    
    free(heap.entries);
    return 0;
}