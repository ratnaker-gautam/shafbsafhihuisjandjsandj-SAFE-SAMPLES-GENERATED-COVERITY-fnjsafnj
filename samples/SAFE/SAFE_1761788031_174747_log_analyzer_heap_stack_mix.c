//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    uint32_t count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) - 1) return 0;
    if (strlen(level) >= sizeof(entry->level) - 1) return 0;
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    entry->level[sizeof(entry->level) - 1] = '\0';
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int compare_entries(const void* a, const void* b) {
    const LogEntry* entry_a = (const LogEntry*)a;
    const LogEntry* entry_b = (const LogEntry*)b;
    return strcmp(entry_a->level, entry_b->level);
}

int main() {
    LogEntry* heap_entries = malloc(MAX_ENTRIES * sizeof(LogEntry));
    if (!heap_entries) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char input_buffer[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    printf("Enter log lines (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a new line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
            break;
        }
        
        input_buffer[strcspn(input_buffer, "\n")] = '\0';
        
        if (strcmp(input_buffer, "END") == 0) {
            break;
        }
        
        if (strlen(input_buffer) == 0) {
            continue;
        }
        
        LogEntry stack_entry;
        if (parse_log_line(input_buffer, &stack_entry)) {
            stack_entry.count = 1;
            heap_entries[entry_count] = stack_entry;
            entry_count++;
        } else {
            fprintf(stderr, "Invalid log format: %s\n", input_buffer);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        free(heap_entries);
        return 0;
    }
    
    qsort(heap_entries, entry_count, sizeof(LogEntry), compare_entries);
    
    LogEntry summary_stack[MAX_ENTRIES];
    int summary_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (summary_count > 0 && 
            strcmp(summary_stack[summary_count - 1].level, heap_entries[i].level) == 0) {
            summary_stack[summary_count - 1].count++;
        } else {
            if (summary_count < MAX_ENTRIES) {
                summary_stack[summary_count] = heap_entries[i];
                summary_count++;
            }
        }
    }
    
    printf("\nLog Summary by Level:\n");
    printf("====================\n");
    
    for (int i = 0; i < summary_count; i++) {
        printf("Level: %-8s Count: %u\n", 
               summary_stack[i].level, summary_stack[i].count);
    }
    
    uint32_t total_count = 0;
    for (int i = 0; i < summary_count; i++) {
        total_count += summary_stack[i].count;
    }
    
    printf("\nTotal log entries: %u\n", total_count);
    
    free(heap_entries);
    return 0;
}