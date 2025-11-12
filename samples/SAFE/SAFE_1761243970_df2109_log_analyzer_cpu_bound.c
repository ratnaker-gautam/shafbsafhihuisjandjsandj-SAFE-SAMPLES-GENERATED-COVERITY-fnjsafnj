//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    entry->error_count = 0;
    if (strstr(entry->level, "ERROR") || strstr(entry->level, "FATAL")) {
        entry->error_count = 1;
    }
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Enter log lines (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            total_errors += entries[entry_count].error_count;
            entry_count++;
        } else {
            printf("Invalid log format. Expected: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total log entries: %d\n", entry_count);
    printf("Total errors: %d\n", total_errors);
    
    if (entry_count > 0) {
        printf("Error rate: %.2f%%\n", (total_errors * 100.0) / entry_count);
        
        int error_entries = 0;
        printf("\nError entries:\n");
        for (int i = 0; i < entry_count && error_entries < 10; i++) {
            if (entries[i].error_count > 0) {
                printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
                error_entries++;
            }
        }
        
        if (error_entries == 0) {
            printf("No error entries found.\n");
        }
    }
    
    return 0;
}