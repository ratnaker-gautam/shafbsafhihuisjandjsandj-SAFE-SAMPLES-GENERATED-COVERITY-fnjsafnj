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
    LogEntry* entries = malloc(MAX_ENTRIES * sizeof(LogEntry));
    if (!entries) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char line_buffer[MAX_LINE_LENGTH];
    uint32_t entry_count = 0;
    
    printf("Enter log lines (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line_buffer, MAX_LINE_LENGTH, stdin)) {
        size_t len = strlen(line_buffer);
        if (len > 0 && line_buffer[len - 1] == '\n') {
            line_buffer[len - 1] = '\0';
        }
        
        if (strlen(line_buffer) == 0) break;
        
        LogEntry new_entry;
        if (parse_log_line(line_buffer, &new_entry)) {
            int found = 0;
            for (uint32_t i = 0; i < entry_count; i++) {
                if (strcmp(entries[i].message, new_entry.message) == 0) {
                    entries[i].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                if (entry_count < MAX_ENTRIES) {
                    entries[entry_count] = new_entry;
                    entry_count++;
                }
            }
        }
    }
    
    qsort(entries, entry_count, sizeof(LogEntry), compare_entries);
    
    printf("\nLog Summary (grouped by message):\n");
    printf("==================================\n");
    
    for (uint32_t i = 0; i < entry_count; i++) {
        if (entries[i].count > 0) {
            printf("Count: %u | Level: %-8s | Message: %s\n", 
                   entries[i].count, entries[i].level, entries[i].message);
        }
    }
    
    uint32_t total_messages = 0;
    for (uint32_t i = 0; i < entry_count; i++) {
        total_messages += entries[i].count;
    }
    
    printf("\nTotal unique messages: %u\n", entry_count);
    printf("Total log entries: %u\n", total_messages);
    
    free(entries);
    return 0;
}