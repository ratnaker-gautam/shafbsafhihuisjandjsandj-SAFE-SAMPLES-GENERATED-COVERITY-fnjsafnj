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
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->count = 1;
    
    return 1;
}

int compare_entries(const void* a, const void* b) {
    const LogEntry* entry_a = (const LogEntry*)a;
    const LogEntry* entry_b = (const LogEntry*)b;
    return strcmp(entry_a->level, entry_b->level);
}

int main(void) {
    LogEntry* entries = malloc(MAX_ENTRIES * sizeof(LogEntry));
    if (!entries) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    printf("Enter log lines (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter empty line to finish input\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n' || line[0] == '\0') break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            fprintf(stderr, "Failed to parse line: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        free(entries);
        return 0;
    }
    
    LogEntry stack_entries[MAX_ENTRIES];
    int stack_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        int found = 0;
        for (int j = 0; j < stack_count; j++) {
            if (strcmp(entries[i].level, stack_entries[j].level) == 0 &&
                strcmp(entries[i].message, stack_entries[j].message) == 0) {
                stack_entries[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found && stack_count < MAX_ENTRIES) {
            stack_entries[stack_count] = entries[i];
            stack_count++;
        }
    }
    
    qsort(stack_entries, stack_count, sizeof(LogEntry), compare_entries);
    
    printf("\nLog Summary:\n");
    printf("Level\t\tCount\tMessage\n");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < stack_count; i++) {
        printf("%-8s\t%u\t%s\n", 
               stack_entries[i].level, 
               stack_entries[i].count,
               stack_entries[i].message);
    }
    
    uint32_t total_count = 0;
    for (int i = 0; i < stack_count; i++) {
        if (UINT32_MAX - total_count < stack_entries[i].count) {
            total_count = UINT32_MAX;
            break;
        }
        total_count += stack_entries[i].count;
    }
    
    printf("\nTotal unique entries: %d\n", stack_count);
    printf("Total log entries: %u\n", total_count);
    
    free(entries);
    return 0;
}