//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[64];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char buffer[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(buffer, line);
    
    char* timestamp = strtok(buffer, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int is_valid_level(const char* level) {
    if (level == NULL) return 0;
    return (strcmp(level, "INFO") == 0 || 
            strcmp(level, "WARN") == 0 || 
            strcmp(level, "ERROR") == 0);
}

void analyze_logs(struct LogEntry* logs, int count) {
    if (logs == NULL || count <= 0) return;
    
    int info_count = 0;
    int warn_count = 0;
    int error_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) info_count++;
        else if (strcmp(logs[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(logs[i].level, "ERROR") == 0) error_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    
    if (count > 0) {
        printf("\nLast 5 entries:\n");
        int start = (count > 5) ? count - 5 : 0;
        for (int i = start; i < count; i++) {
            printf("%s [%s] %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
        }
    }
}

int read_sample_logs(struct LogEntry* logs) {
    const char* sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:35:23 WARN High memory usage detected",
        "2024-01-15T10:40:15 INFO User login: john_doe",
        "2024-01-15T10:45:01 ERROR Database connection failed",
        "2024-01-15T10:50:34 INFO Backup completed",
        "2024-01-15T10:55:12 WARN Disk space running low",
        "2024-01-15T11:00:45 INFO Scheduled task executed",
        "2024-01-15T11:05:33 ERROR File not found: config.xml",
        "2024-01-15T11:10:28 INFO Cache cleared",
        "2024-01-15T11:15:19 INFO System shutdown initiated"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    int valid_count = 0;
    
    for (int i = 0; i < sample_count && valid_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &logs[valid_count])) {
            if (is_valid_level(logs[valid_count].level)) {
                valid_count++;
            }
        }
    }
    
    return valid_count;
}

int main() {
    struct LogEntry logs[MAX_ENTRIES];
    
    printf("Log Analyzer\n");
    printf("============\n\n");
    
    int log_count = read_sample_logs(logs);
    
    if (log_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(logs, log_count);
    
    return 0;
}