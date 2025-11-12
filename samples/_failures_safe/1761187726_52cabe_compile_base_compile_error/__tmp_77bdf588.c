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
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* parts[3];
    char* token = strtok(temp_line, "|");
    int part_count = 0;
    
    while (token && part_count < 3) {
        parts[part_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
    if (strlen(parts[0]) >= sizeof(entry->timestamp) ||
        strlen(parts[1]) >= sizeof(entry->level) ||
        strlen(parts[2]) >= sizeof(entry->message)) {
        return 0;
    }
    
    strncpy(entry->timestamp, parts[0], sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, parts[1], sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, parts[2], sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    entry->count = 1;
    
    return 1;
}

int compare_entries(const void* a, const void* b) {
    const LogEntry* entry_a = (const LogEntry*)a;
    const LogEntry* entry_b = (const LogEntry*)b;
    return strcmp(entry_a->level, entry_b->level);
}

int main() {
    LogEntry stack_entries[MAX_ENTRIES];
    LogEntry* heap_entries = NULL;
    int entry_count = 0;
    char input_line[MAX_LINE_LENGTH];
    
    printf("Enter log lines (format: timestamp|level|message):\n");
    printf("Enter 'END' to finish input.\n");
    
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
        
        if (parse_log_line(input_line, &stack_entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: timestamp|level|message\n");
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
        memcpy(&heap_entries[i], &stack_entries[i], sizeof(LogEntry));
    }
    
    qsort(heap_entries, entry_count, sizeof(LogEntry), compare_entries);
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    char current_level[16] = "";
    uint32_t level_count = 0;
    uint32_t total_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(heap_entries[i].level, current_level) != 0) {
            if (level_count > 0) {
                printf("Level %s: %u entries\n", current_level, level_count);
                total_count += level_count;
            }
            strncpy(current_level, heap_entries[i].level, sizeof(current_level) - 1);
            current_level[sizeof(current_level) - 1] = '\0';
            level_count = 1;
        } else {
            level_count++;
        }
    }
    
    if (level_count > 0) {
        printf("Level %s: %u entries\n", current_level, level_count);
        total_count += level_count;
    }
    
    printf("Total entries: %u\n", total_count);
    
    printf("\nMost frequent messages:\n");
    uint32_t max_count = 0;
    for (int i = 0; i < entry_count; i++) {
        if (heap_entries[i].count > max_count) {
            max_count = heap_entries[i].count;
        }
    }
    
    int printed = 0;
    for (int i = 0; i < entry_count && printed < 5; i++) {
        if (heap_entries[i].count == max_count) {