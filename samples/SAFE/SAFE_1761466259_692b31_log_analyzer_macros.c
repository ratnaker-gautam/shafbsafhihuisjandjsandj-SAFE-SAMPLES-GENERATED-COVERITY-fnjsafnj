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

#define IS_VALID_LEVEL(l) (strcmp(l, "INFO") == 0 || strcmp(l, "WARNING") == 0 || strcmp(l, "ERROR") == 0)

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

#define COUNT_LEVEL(e, l, c) if (strcmp((e).level, l) == 0) c++

void analyze_logs(LogEntry logs[], int count) {
    int info_count = 0, warning_count = 0, error_count = 0;
    
    for (int i = 0; i < count; i++) {
        COUNT_LEVEL(logs[i], "INFO", info_count);
        COUNT_LEVEL(logs[i], "WARNING", warning_count);
        COUNT_LEVEL(logs[i], "ERROR", error_count);
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    
    if (count > 0) {
        printf("Error percentage: %.2f%%\n", (error_count * 100.0) / count);
    }
}

#define GENERATE_SAMPLE_LOG() do { \
    printf("2024-01-15T10:30:00 INFO System started successfully\n"); \
    printf("2024-01-15T10:31:15 WARNING High memory usage detected\n"); \
    printf("2024-01-15T10:32:45 ERROR Database connection failed\n"); \
    printf("2024-01-15T10:33:10 INFO Backup process completed\n"); \
    printf("2024-01-15T10:34:20 ERROR File not found: config.ini\n"); \
} while(0)

int main() {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char input[MAX_LINE_LEN];
    
    printf("Log Analyzer\n");
    printf("Enter log entries (one per line, format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARNING, ERROR\n");
    printf("Enter 'sample' to load sample data, 'done' to finish input\n");
    
    while (log_count < MAX_ENTRIES) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) break;
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "done") == 0) break;
        
        if (strcmp(input, "sample") == 0) {
            GENERATE_SAMPLE_LOG();
            continue;
        }
        
        if (strlen(input) == 0) continue;
        
        LogEntry entry;
        if (parse_log_entry(input, &entry)) {
            logs[log_count++] = entry;
            printf("Added: %s %s %s\n", entry.timestamp, entry.level, entry.message);
        } else {
            printf("Invalid log entry format\n");
        }
    }
    
    if (log_count > 0) {
        analyze_logs(logs, log_count);
    } else {
        printf("No valid log entries to analyze\n");
    }
    
    return 0;
}