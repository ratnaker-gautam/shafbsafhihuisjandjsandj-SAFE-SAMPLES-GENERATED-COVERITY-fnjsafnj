//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char* token = strtok(temp_line, " ");
    if (!token) return 0;
    
    if (strlen(token) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, token);
    
    token = strtok(NULL, " ");
    if (!token) return 0;
    
    if (strlen(token) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, token);
    
    token = strtok(NULL, "\n");
    if (!token) return 0;
    
    if (strlen(token) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, token);
    
    if (strcmp(entry->level, "ERROR") == 0) entry->severity = 3;
    else if (strcmp(entry->level, "WARN") == 0) entry->severity = 2;
    else if (strcmp(entry->level, "INFO") == 0) entry->severity = 1;
    else entry->severity = 0;
    
    return 1;
}

int compare_entries(const void* a, const void* b) {
    const struct LogEntry* entry_a = (const struct LogEntry*)a;
    const struct LogEntry* entry_b = (const struct LogEntry*)b;
    return entry_b->severity - entry_a->severity;
}

int main(void) {
    struct LogEntry entries_stack[MAX_ENTRIES];
    struct LogEntry* entries_heap = NULL;
    int entry_count = 0;
    char input_line[MAX_LINE_LEN];
    
    printf("Enter log entries (one per line, format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(input_line, sizeof(input_line), stdin)) break;
        
        input_line[strcspn(input_line, "\n")] = 0;
        
        if (strcmp(input_line, "END") == 0) break;
        
        if (strlen(input_line) == 0) continue;
        
        if (parse_log_line(input_line, &entries_stack[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", input_line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    entries_heap = malloc(entry_count * sizeof(struct LogEntry));
    if (!entries_heap) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    for (int i = 0; i < entry_count; i++) {
        entries_heap[i] = entries_stack[i];
    }
    
    qsort(entries_heap, entry_count, sizeof(struct LogEntry), compare_entries);
    
    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (entries_heap[i].severity == 3) error_count++;
        else if (entries_heap[i].severity == 2) warn_count++;
        else if (entries_heap[i].severity == 1) info_count++;
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARN entries: %d\n", warn_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nEntries sorted by severity (highest first):\n");
    for (int i = 0; i < entry_count; i++) {
        printf("%s %s %s\n", entries_heap[i].timestamp, entries_heap[i].level, entries_heap[i].message);
    }
    
    free(entries_heap);
    return 0;
}