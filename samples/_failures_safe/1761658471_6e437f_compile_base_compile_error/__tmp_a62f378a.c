//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
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
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp_start = strchr(temp_line, '[');
    if (!timestamp_start) return 0;
    
    char* timestamp_end = strchr(timestamp_start, ']');
    if (!timestamp_end) return 0;
    
    size_t timestamp_len = timestamp_end - timestamp_start - 1;
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    
    strncpy(entry->timestamp, timestamp_start + 1, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    char* level_start = strchr(timestamp_end + 1, '[');
    if (!level_start) return 0;
    
    char* level_end = strchr(level_start, ']');
    if (!level_end) return 0;
    
    size_t level_len = level_end - level_start - 1;
    if (level_len >= sizeof(entry->level)) return 0;
    
    strncpy(entry->level, level_start + 1, level_len);
    entry->level[level_len] = '\0';
    
    char* message_start = level_end + 1;
    while (*message_start == ' ' || *message_start == '\t') message_start++;
    
    size_t message_len = strlen(message_start);
    if (message_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, message_start, message_len);
    entry->message[message_len] = '\0';
    
    return 1;
}

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || 
           strcmp(level, "WARN") == 0 || 
           strcmp(level, "ERROR") == 0 ||
           strcmp(level, "DEBUG") == 0;
}

void analyze_logs(LogEntry* logs, int count) {
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    int error_indices[MAX_ENTRIES];
    int error_count_total = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) info_count++;
        else if (strcmp(logs[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(logs[i].level, "ERROR") == 0) {
            error_count++;
            if (error_count_total < MAX_ENTRIES) {
                error_indices[error_count_total++] = i;
            }
        }
        else if (strcmp(logs[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    if (error_count_total > 0) {
        printf("\nError entries:\n");
        for (int i = 0; i < error_count_total && i < 10; i++) {
            int idx = error_indices[i];
            printf("[%s] [%s] %s\n", logs[idx].timestamp, logs[idx].level, logs[idx].message);
        }
        if (error_count_total > 10) {
            printf("... and %d more errors\n", error_count_total - 10);
        }
    }
}

int main() {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    printf("Enter log entries (format: [timestamp] [LEVEL] message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LENGTH];
    while (log_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
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
    
    analyze_logs(logs, log_count);
    
    return 0