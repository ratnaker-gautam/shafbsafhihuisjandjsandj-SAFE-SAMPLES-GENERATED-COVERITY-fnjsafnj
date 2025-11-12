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
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log lines (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n') break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) continue;
        
        LogEntry new_entry;
        if (parse_log_line(line, &new_entry)) {
            int found = 0;
            for (int i = 0; i < entry_count; i++) {
                if (strcmp(stack_entries[i].message, new_entry.message) == 0) {
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
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
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
    
    printf("\nLog Summary (sorted by message):\n");
    printf("================================\n");
    
    uint32_t total_count = 0;
    for (int i = 0; i < entry_count; i++) {
        printf("%s [%s]: %s (count: %u)\n", 
               heap_entries[i].timestamp, 
               heap_entries[i].level, 
               heap_entries[i].message, 
               heap_entries[i].count);
        total_count += heap_entries[i].count;
    }
    
    printf("\nTotal unique messages: %d\n", entry_count);
    printf("Total log entries processed: %u\n", total_count);
    
    free(heap_entries);
    return 0;
}