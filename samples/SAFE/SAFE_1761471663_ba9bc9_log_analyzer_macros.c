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
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return IS_VALID_LEVEL(entry->level);
}

#define COUNT_LEVEL(entries, count, target_level) \
    do { \
        for (size_t i = 0; i < count; i++) { \
            if (strcmp(entries[i].level, target_level) == 0) { \
                printf("  %s: %s\n", entries[i].timestamp, entries[i].message); \
            } \
        } \
    } while(0)

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    size_t entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARNING, ERROR\n");
    printf("Enter 'END' on a new line to finish input.\n\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_entry(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log entry format: %s\n", line);
        }
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries: %zu\n\n", entry_count);
    
    int info_count = 0, warning_count = 0, error_count = 0;
    for (size_t i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
    }
    
    printf("Level Statistics:\n");
    printf("  INFO: %d\n", info_count);
    printf("  WARNING: %d\n", warning_count);
    printf("  ERROR: %d\n\n", error_count);
    
    if (error_count > 0) {
        printf("ERROR entries:\n");
        COUNT_LEVEL(entries, entry_count, "ERROR");
    }
    
    if (warning_count > 0) {
        printf("\nWARNING entries:\n");
        COUNT_LEVEL(entries, entry_count, "WARNING");
    }
    
    return 0;
}