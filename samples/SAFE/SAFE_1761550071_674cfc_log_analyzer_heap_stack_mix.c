//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[64];
    char level[16];
    char message[256];
    int severity;
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char level_buf[16];
    char timestamp_buf[64];
    char message_buf[256];
    
    int parsed = sscanf(line, "%63s %15s %255[^\n]", 
                       timestamp_buf, level_buf, message_buf);
    
    if (parsed != 3) return 0;
    
    if (strlen(timestamp_buf) >= sizeof(entry->timestamp) ||
        strlen(level_buf) >= sizeof(entry->level) ||
        strlen(message_buf) >= sizeof(entry->message)) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp_buf);
    strcpy(entry->level, level_buf);
    strcpy(entry->message, message_buf);
    
    if (strcmp(level_buf, "ERROR") == 0) entry->severity = 3;
    else if (strcmp(level_buf, "WARN") == 0) entry->severity = 2;
    else if (strcmp(level_buf, "INFO") == 0) entry->severity = 1;
    else entry->severity = 0;
    
    return 1;
}

int compare_entries(const void* a, const void* b) {
    const struct LogEntry* entry_a = (const struct LogEntry*)a;
    const struct LogEntry* entry_b = (const struct LogEntry*)b;
    
    int severity_cmp = entry_b->severity - entry_a->severity;
    if (severity_cmp != 0) return severity_cmp;
    
    return strcmp(entry_a->timestamp, entry_b->timestamp);
}

int main(void) {
    struct LogEntry* heap_entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (heap_entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    struct LogEntry stack_entries[100];
    int heap_count = 0;
    int stack_count = 0;
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && line_count < 1100) {
        line_count++;
        if (strlen(line) >= MAX_LINE_LENGTH - 1) continue;
        
        struct LogEntry entry;
        if (!parse_log_line(line, &entry)) continue;
        
        if (entry.severity >= 2) {
            if (heap_count < MAX_ENTRIES) {
                heap_entries[heap_count] = entry;
                heap_count++;
            }
        } else {
            if (stack_count < 100) {
                stack_entries[stack_count] = entry;
                stack_count++;
            }
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        free(heap_entries);
        return 1;
    }
    
    qsort(heap_entries, heap_count, sizeof(struct LogEntry), compare_entries);
    
    printf("High Severity Logs (Heap):\n");
    for (int i = 0; i < heap_count; i++) {
        printf("%s %s %s\n", heap_entries[i].timestamp, 
               heap_entries[i].level, heap_entries[i].message);
    }
    
    printf("\nLow Severity Logs (Stack):\n");
    for (int i = 0; i < stack_count; i++) {
        printf("%s %s %s\n", stack_entries[i].timestamp, 
               stack_entries[i].level, stack_entries[i].message);
    }
    
    printf("\nSummary:\n");
    printf("Total lines processed: %d\n", line_count);
    printf("High severity entries: %d\n", heap_count);
    printf("Low severity entries: %d\n", stack_count);
    
    free(heap_entries);
    return 0;
}