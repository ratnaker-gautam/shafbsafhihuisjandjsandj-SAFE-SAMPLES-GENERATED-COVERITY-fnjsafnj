//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->count = 1;
    
    return 1;
}

int log_heap_init(LogHeap *heap, size_t initial_capacity) {
    if (!heap || initial_capacity == 0) return 0;
    
    heap->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!heap->entries) return 0;
    
    heap->count = 0;
    heap->capacity = initial_capacity;
    return 1;
}

void log_heap_free(LogHeap *heap) {
    if (heap && heap->entries) {
        free(heap->entries);
        heap->entries = NULL;
        heap->count = 0;
        heap->capacity = 0;
    }
}

int log_heap_add(LogHeap *heap, const LogEntry *entry) {
    if (!heap || !entry || heap->count >= heap->capacity) return 0;
    
    if (heap->count < heap->capacity) {
        heap->entries[heap->count] = *entry;
        heap->count++;
        return 1;
    }
    return 0;
}

void process_log_stack(LogHeap *heap) {
    if (!heap || heap->count == 0) return;
    
    LogEntry stack_entries[50];
    size_t stack_count = 0;
    
    for (size_t i = 0; i < heap->count && stack_count < 50; i++) {
        int found = 0;
        for (size_t j = 0; j < stack_count; j++) {
            if (strcmp(heap->entries[i].level, stack_entries[j].level) == 0) {
                stack_entries[j].count += heap->entries[i].count;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(stack_entries[stack_count].level, heap->entries[i].level);
            stack_entries[stack_count].count = heap->entries[i].count;
            stack_count++;
        }
    }
    
    printf("Log level summary:\n");
    for (size_t i = 0; i < stack_count; i++) {
        printf("Level: %s, Count: %u\n", stack_entries[i].level, stack_entries[i].count);
    }
}

int main() {
    LogHeap heap;
    if (!log_heap_init(&heap, 100)) {
        fprintf(stderr, "Failed to initialize log heap\n");
        return 1;
    }
    
    char input_buffer[MAX_LINE_LEN];
    printf("Enter log lines (format: timestamp level message). Enter 'END' to finish:\n");
    
    while (fgets(input_buffer, sizeof(input_buffer), stdin)) {
        if (strncmp(input_buffer, "END", 3) == 0) break;
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len-1] == '\n') {
            input_buffer[len-1] = '\0';
        }
        
        if (strlen(input_buffer) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(input_buffer, &entry)) {
            if (!log_heap_add(&heap, &entry)) {
                fprintf(stderr, "Failed to add log entry\n");
                break;
            }
        } else {
            fprintf(stderr, "Failed to parse log line: %s\n", input_buffer);
        }
        
        if (heap.count >= heap.capacity) {
            fprintf(stderr, "Log capacity reached\n");
            break;
        }
    }
    
    if (heap.count > 0) {
        printf("\nProcessing %zu log entries...\n", heap.count);
        process_log_stack(&heap);
        
        printf("\nAll log entries:\n");
        for (size_t i = 0; i < heap.count; i++) {
            printf("%s %s %s\n", heap.entries[i].timestamp, heap.entries[i].level, heap.entries[i].message);
        }
    } else {
        printf("No valid log entries processed\n");
    }
    
    log_heap_free(&heap);
    return 0;
}