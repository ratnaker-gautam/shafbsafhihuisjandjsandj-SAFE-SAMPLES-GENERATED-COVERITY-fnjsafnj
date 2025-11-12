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
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
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
    return strcmp(level, "INFO") == 0 || 
           strcmp(level, "WARN") == 0 || 
           strcmp(level, "ERROR") == 0;
}

void analyze_logs(LogEntry* logs, int count) {
    int info_count = 0, warn_count = 0, error_count = 0;
    int total_length = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) info_count++;
        else if (strcmp(logs[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(logs[i].level, "ERROR") == 0) error_count++;
        
        total_length += strlen(logs[i].message);
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    
    if (count > 0) {
        double avg_length = (double)total_length / count;
        printf("Average message length: %.2f\n", avg_length);
    }
}

int main(void) {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a new line to finish input.\n");
    
    char line[MAX_LINE_LENGTH];
    while (log_count < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) {
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
                printf("Invalid log level: %s\n", entry.level);
            }
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