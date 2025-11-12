//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    
    entry->count = 1;
    
    return 1;
}

int compare_entries(const void* a, const void* b) {
    const LogEntry* entry_a = (const LogEntry*)a;
    const LogEntry* entry_b = (const LogEntry*)b;
    return strcmp(entry_a->level, entry_b->level);
}

int main(void) {
    LogEntry stack_entries[MAX_ENTRIES];
    LogEntry* heap_entries = NULL;
    int entry_count = 0;
    char input_line[MAX_LINE_LENGTH];
    
    printf("Enter log lines (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(input_line, sizeof(input_line), stdin)) {
            break;
        }
        
        input_line[strcspn(input_line, "\n")] = '\0';
        
        if (strcmp(input_line, "END") == 0) {
            break;
        }
        
        if (strlen(input_line) == 0) {
            continue;
        }
        
        LogEntry new_entry;
        if (parse_log_line(input_line, &new_entry)) {
            int found = 0;
            for (int i = 0; i < entry_count; i++) {
                if (strcmp(stack_entries[i].timestamp, new_entry.timestamp) == 0 &&
                    strcmp(stack_entries[i].level, new_entry.level) == 0 &&
                    strcmp(stack_entries[i].message, new_entry.message) == 0) {
                    stack_entries[i].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                if (entry_count < MAX_ENTRIES) {
                    stack_entries[entry_count] = new_entry;
                    entry_count++;
                }
            }
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    heap_entries = malloc(entry_count * sizeof(LogEntry));
    if (!heap_entries) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    for (int i = 0; i < entry_count; i++) {
        heap_entries[i] = stack_entries[i];
    }
    
    qsort(heap_entries, entry_count, sizeof(LogEntry), compare_entries);
    
    printf("\nLog Analysis Summary:\n");
    printf("====================\n");
    
    char current_level[16] = "";
    uint32_t level_count = 0;
    uint32_t total_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(current_level, heap_entries[i].level) != 0) {
            if (strlen(current_level) > 0) {
                printf("Level %s: %u entries\n", current_level, level_count);
            }
            strncpy(current_level, heap_entries[i].level, sizeof(current_level) - 1);
            current_level[sizeof(current_level) - 1] = '\0';
            level_count = 0;
        }
        level_count += heap_entries[i].count;
        total_count += heap_entries[i].count;
    }
    
    if (strlen(current_level) > 0) {
        printf("Level %s: %u entries\n", current_level, level_count);