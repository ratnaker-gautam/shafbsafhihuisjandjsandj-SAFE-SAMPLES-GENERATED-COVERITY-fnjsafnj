//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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

int parse_log_line(const char* line, LogEntry* entry) {
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

void analyze_logs(LogEntry* logs, int count) {
    if (!logs || count <= 0) return;
    
    int info_count = 0, warning_count = 0, error_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) info_count++;
        else if (strcmp(logs[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(logs[i].level, "ERROR") == 0) error_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    
    if (count > 0) {
        printf("\nFirst timestamp: %s\n", logs[0].timestamp);
        printf("Last timestamp: %s\n", logs[count-1].timestamp);
    }
}

int main() {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Supported levels: INFO, WARNING, ERROR\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    while (log_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            logs[log_count] = entry;
            log_count++;
            printf("Added: %s %s %s\n", entry.timestamp, entry.level, entry.message);
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (log_count > 0) {
        analyze_logs(logs, log_count);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}