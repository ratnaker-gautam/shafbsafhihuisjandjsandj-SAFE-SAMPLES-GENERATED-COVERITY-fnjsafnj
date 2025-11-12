//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_errors(LogEntry* entry) {
    static int error_count = 0;
    if (strcmp(entry->level, "ERROR") == 0) {
        error_count++;
        printf("Error #%d: %s - %s\n", error_count, entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARNING") == 0) {
        printf("Warning: %s - %s\n", entry->timestamp, entry->message);
    }
}

void display_all(LogEntry* entry) {
    printf("[%s] %s: %s\n", entry->timestamp, entry->level, entry->message);
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    size_t len = strlen(line);
    if (len == 0 || len >= MAX_LINE_LENGTH) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void process_logs(LogEntry entries[], int count, LogProcessor processor) {
    if (entries == NULL || processor == NULL || count <= 0 || count > MAX_ENTRIES) return;
    
    for (int i = 0; i < count; i++) {
        processor(&entries[i]);
    }
}

int main(void) {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    const char* sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:35:23 WARNING High memory usage detected",
        "2024-01-15T10:40:15 ERROR Database connection failed",
        "2024-01-15T10:42:00 INFO Backup completed",
        "2024-01-15T10:45:30 WARNING CPU temperature rising",
        "2024-01-15T10:50:00 ERROR File not found: config.xml",
        "2024-01-15T10:55:10 INFO User login successful",
        "2024-01-15T11:00:00 WARNING Disk space low",
        "2024-01-15T11:05:45 ERROR Network timeout",
        "2024-01-15T11:10:20 INFO System shutdown initiated"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && log_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &logs[log_count])) {
            log_count++;
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries to process.\n");
        return 1;
    }
    
    printf("Processing all log entries:\n");
    printf("===========================\n");
    process_logs(logs, log_count, display_all);
    
    printf("\nError summary:\n");
    printf("==============\n");
    process_logs(logs, log_count, count_errors);
    
    printf("\nWarning summary:\n");
    printf("================\n");
    process_logs(logs, log_count, show_warnings);
    
    return 0;
}