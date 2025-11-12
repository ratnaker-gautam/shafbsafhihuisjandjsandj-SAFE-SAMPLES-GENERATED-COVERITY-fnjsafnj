//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int error_count;
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
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Processing log entries...\n");
    
    while (fgets(line, sizeof(line), stdin) != NULL && entry_count < MAX_ENTRIES) {
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            total_errors += entry.error_count;
            entry_count++;
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries processed.\n");
        return 1;
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Total errors found: %d\n", total_errors);
    
    if (total_errors > 0) {
        printf("\nError entries:\n");
        for (int i = 0; i < entry_count; i++) {
            if (entries[i].error_count > 0) {
                printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            }
        }
    }
    
    int info_count = 0;
    int warn_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
    }
    
    printf("\nLevel distribution:\n");
    printf("INFO: %d (%.1f%%)\n", info_count, (info_count * 100.0) / entry_count);
    printf("WARN: %d (%.1f%%)\n", warn_count, (warn_count * 100.0) / entry_count);
    printf("ERROR: %d (%.1f%%)\n", total_errors, (total_errors * 100.0) / entry_count);
    
    return 0;
}