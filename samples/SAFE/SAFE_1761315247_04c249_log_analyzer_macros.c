//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LEVEL_LEN 16

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN];
} LogEntry;

#define IS_VALID_LEVEL(lvl) (strcmp(lvl, "INFO") == 0 || strcmp(lvl, "WARNING") == 0 || strcmp(lvl, "ERROR") == 0)

int parse_log_entry(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "");
    
    if (!timestamp || !level || !message) return 0;
    if (strlen(timestamp) >= TIMESTAMP_LEN - 1) return 0;
    if (strlen(level) >= LEVEL_LEN - 1) return 0;
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    
    if (!IS_VALID_LEVEL(level)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void analyze_logs(LogEntry* entries, int count) {
    int info_count = 0, warning_count = 0, error_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    
    if (count > 0) {
        printf("\nFirst entry: %s %s %s\n", entries[0].timestamp, entries[0].level, entries[0].message);
        printf("Last entry: %s %s %s\n", entries[count-1].timestamp, entries[count-1].level, entries[count-1].message);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Press Ctrl+D (Linux) or Ctrl+Z (Windows) to finish input.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_entry(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    analyze_logs(entries, entry_count);
    
    return 0;
}