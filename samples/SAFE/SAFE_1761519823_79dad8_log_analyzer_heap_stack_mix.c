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
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->count = 1;
    
    return 1;
}

int compare_entries(const void* a, const void* b) {
    const LogEntry* entry_a = (const LogEntry*)a;
    const LogEntry* entry_b = (const LogEntry*)b;
    return strcmp(entry_a->message, entry_b->message);
}

int main() {
    LogEntry stack_entries[MAX_ENTRIES];
    LogEntry* heap_entries = NULL;
    int entry_count = 0;
    char input_line[MAX_LINE_LENGTH];
    
    printf("Enter log lines (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(input_line, MAX_LINE_LENGTH, stdin)) break;
        
        size_t len = strlen(input_line);
        if (len > 0 && input_line[len - 1] == '\n') {
            input_line[len - 1] = '\0';
        }
        
        if (strlen(input_line) == 0) break;
        
        if (parse_log_line(input_line, &stack_entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping.\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    heap_entries = malloc(entry_count * sizeof(LogEntry));
    if (!heap_entries) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    for (int i = 0; i < entry_count; i++) {
        memcpy(&heap_entries[i], &stack_entries[i], sizeof(LogEntry));
    }
    
    qsort(heap_entries, entry_count, sizeof(LogEntry), compare_entries);
    
    int unique_count = 0;
    for (int i = 0; i < entry_count; i++) {
        if (i == 0 || strcmp(heap_entries[i].message, heap_entries[i - 1].message) != 0) {
            unique_count++;
        } else {
            heap_entries[unique_count - 1].count++;
        }
    }
    
    printf("\nLog Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("Unique messages: %d\n\n", unique_count);
    
    printf("Message frequencies:\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %u occurrences\n", heap_entries[i].message, heap_entries[i].count);
    }
    
    free(heap_entries);
    return 0;
}