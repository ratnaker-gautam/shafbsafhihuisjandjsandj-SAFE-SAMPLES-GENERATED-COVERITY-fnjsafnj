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
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    if (!timestamp) return 0;
    
    char* level = strtok(NULL, " ");
    if (!level) return 0;
    
    char* message = strtok(NULL, "\n");
    if (!message) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || 
           strcmp(level, "WARN") == 0 || 
           strcmp(level, "ERROR") == 0;
}

void analyze_logs(LogEntry* logs, int count) {
    int info_count = 0, warn_count = 0, error_count = 0;
    char error_messages[MAX_ENTRIES][256];
    int error_count_per_msg[MAX_ENTRIES] = {0};
    int unique_errors = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(logs[i].level, "WARN") == 0) {
            warn_count++;
        } else if (strcmp(logs[i].level, "ERROR") == 0) {
            error_count++;
            
            int found = 0;
            for (int j = 0; j < unique_errors; j++) {
                if (strcmp(error_messages[j], logs[i].message) == 0) {
                    error_count_per_msg[j]++;
                    found = 1;
                    break;
                }
            }
            if (!found && unique_errors < MAX_ENTRIES) {
                strncpy(error_messages[unique_errors], logs[i].message, 255);
                error_messages[unique_errors][255] = '\0';
                error_count_per_msg[unique_errors] = 1;
                unique_errors++;
            }
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    
    if (error_count > 0) {
        printf("\nMost frequent error messages:\n");
        for (int i = 0; i < unique_errors && i < 5; i++) {
            printf("%d occurrences: %s\n", error_count_per_msg[i], error_messages[i]);
        }
    }
}

int main() {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO User logged in\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LENGTH];
    while (log_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) {
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
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    clock_t start = clock();
    analyze_logs(logs, log_count);
    clock_t end = clock();
    
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nAnalysis completed in %.6f seconds\n", cpu_time);
    
    return 0;
}