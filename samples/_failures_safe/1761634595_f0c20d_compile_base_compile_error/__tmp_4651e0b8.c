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
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
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
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    int heap_size = 0;
    
    printf("Enter log lines (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a new line to finish input.\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        if (entry_count < MAX_ENTRIES) {
            if (parse_log_line(line, &stack_entries[entry_count])) {
                entry_count++;
            }
        } else {
            if (heap_size == 0) {
                heap_entries = malloc(MAX_ENTRIES * sizeof(LogEntry));
                if (!heap_entries) {
                    fprintf(stderr, "Memory allocation failed\n");
                    return 1;
                }
                heap_size = MAX_ENTRIES;
            }
            
            if (entry_count - MAX_ENTRIES < heap_size) {
                if (parse_log_line(line, &heap_entries[entry_count - MAX_ENTRIES])) {
                    entry_count++;
                }
            } else {
                LogEntry* new_heap = realloc(heap_entries, (heap_size + MAX_ENTRIES) * sizeof(LogEntry));
                if (!new_heap) {
                    fprintf(stderr, "Memory reallocation failed\n");
                    free(heap_entries);
                    return 1;
                }
                heap_entries = new_heap;
                heap_size += MAX_ENTRIES;
                
                if (parse_log_line(line, &heap_entries[entry_count - MAX_ENTRIES])) {
                    entry_count++;
                }
            }
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        free(heap_entries);
        return 0;
    }
    
    LogEntry* all_entries = malloc(entry_count * sizeof(LogEntry));
    if (!all_entries) {
        fprintf(stderr, "Memory allocation failed\n");
        free(heap_entries);
        return 1;
    }
    
    for (int i = 0; i < entry_count; i++) {
        if (i < MAX_ENTRIES) {
            all_entries[i] = stack_entries[i];
        } else {
            all_entries[i] = heap_entries[i - MAX_ENTRIES];
        }
    }
    
    qsort(all_entries, entry_count, sizeof(LogEntry), compare_entries);
    
    printf("\nLog entries sorted by level:\n");
    printf("============================\n");
    
    char current_level[16] = "";
    uint32_t level_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(all_entries[i].level, current_level) != 0) {
            if (level_count > 0) {
                printf("Total entries for %s: