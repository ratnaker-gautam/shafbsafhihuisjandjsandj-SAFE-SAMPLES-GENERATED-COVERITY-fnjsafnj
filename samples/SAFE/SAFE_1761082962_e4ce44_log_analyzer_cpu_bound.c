//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* token = strtok(temp, " ");
    if (token == NULL) return 0;
    strncpy(entry->timestamp, token, 31);
    entry->timestamp[31] = '\0';
    
    token = strtok(NULL, " ");
    if (token == NULL) return 0;
    strncpy(entry->level, token, 15);
    entry->level[15] = '\0';
    
    token = strtok(NULL, "\n");
    if (token == NULL) return 0;
    strncpy(entry->message, token, 255);
    entry->message[255] = '\0';
    
    return 1;
}

int is_valid_level(const char* level) {
    return (strcmp(level, "INFO") == 0 || 
            strcmp(level, "WARN") == 0 || 
            strcmp(level, "ERROR") == 0 ||
            strcmp(level, "DEBUG") == 0);
}

void analyze_logs(LogEntry* logs, int count) {
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) info_count++;
        else if (strcmp(logs[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(logs[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(logs[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);
    printf("ERROR entries: %d\n", error_count);
    printf("DEBUG entries: %d\n", debug_count);
    printf("Total entries: %d\n", count);
    
    if (count > 0) {
        printf("Distribution:\n");
        printf("INFO: %.2f%%\n", (info_count * 100.0) / count);
        printf("WARN: %.2f%%\n", (warn_count * 100.0) / count);
        printf("ERROR: %.2f%%\n", (error_count * 100.0) / count);
        printf("DEBUG: %.2f%%\n", (debug_count * 100.0) / count);
    }
}

int main() {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO User logged in\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (log_count < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LEN, stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (is_valid_level(entry.level)) {
                logs[log_count] = entry;
                log_count++;
            } else {
                printf("Invalid log level in: %s\n", line);
            }
        } else {
            printf("Failed to parse: %s\n", line);
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nProcessing %d log entries...\n", log_count);
    analyze_logs(logs, log_count);
    
    return 0;
}