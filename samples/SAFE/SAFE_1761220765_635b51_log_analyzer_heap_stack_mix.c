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

int parse_log_line(const char *line, LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char level[16];
    char message[256];
    char timestamp[32];
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (parsed != 3) return 0;
    
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int compare_entries(const void *a, const void *b) {
    const LogEntry *entry_a = (const LogEntry *)a;
    const LogEntry *entry_b = (const LogEntry *)b;
    return strcmp(entry_a->timestamp, entry_b->timestamp);
}

int main(void) {
    LogHeap heap;
    heap.count = 0;
    heap.capacity = 10;
    heap.entries = malloc(heap.capacity * sizeof(LogEntry));
    if (!heap.entries) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    while (fgets(line, sizeof(line), stdin)) {
        line_count++;
        if (line_count > MAX_ENTRIES) {
            fprintf(stderr, "Too many log entries\n");
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (heap.count >= heap.capacity) {
                int new_capacity = heap.capacity * 2;
                if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
                LogEntry *new_entries = realloc(heap.entries, new_capacity * sizeof(LogEntry));
                if (!new_entries) {
                    fprintf(stderr, "Memory reallocation failed\n");
                    break;
                }
                heap.entries = new_entries;
                heap.capacity = new_capacity;
            }
            
            heap.entries[heap.count] = entry;
            heap.count++;
        } else {
            fprintf(stderr, "Invalid log format: %s\n", line);
        }
    }
    
    if (heap.count > 0) {
        qsort(heap.entries, heap.count, sizeof(LogEntry), compare_entries);
        
        printf("\nSorted log entries:\n");
        printf("==================\n");
        
        int error_count = 0;
        int warning_count = 0;
        int info_count = 0;
        
        for (int i = 0; i < heap.count; i++) {
            printf("%s %s %s\n", heap.entries[i].timestamp, heap.entries[i].level, heap.entries[i].message);
            
            if (strcmp(heap.entries[i].level, "ERROR") == 0) {
                error_count++;
            } else if (strcmp(heap.entries[i].level, "WARNING") == 0) {
                warning_count++;
            } else if (strcmp(heap.entries[i].level, "INFO") == 0) {
                info_count++;
            }
        }
        
        printf("\nSummary:\n");
        printf("Total entries: %d\n", heap.count);
        printf("ERROR entries: %d\n", error_count);
        printf("WARNING entries: %d\n", warning_count);
        printf("INFO entries: %d\n", info_count);
    } else {
        printf("No valid log entries to process\n");
    }
    
    free(heap.entries);
    return 0;
}