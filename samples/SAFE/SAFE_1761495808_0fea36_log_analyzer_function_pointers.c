//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[64];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_errors(LogEntry* entry) {
    static int error_count = 0;
    if (strcmp(entry->level, "ERROR") == 0) {
        error_count++;
        printf("Error count: %d\n", error_count);
    }
}

void print_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARNING") == 0) {
        printf("Warning: %s\n", entry->message);
    }
}

void find_keyword(LogEntry* entry) {
    const char* keyword = "failed";
    if (strstr(entry->message, keyword) != NULL) {
        printf("Found '%s' in log: %s\n", keyword, entry->timestamp);
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    size_t len = strlen(line);
    if (len == 0 || len >= MAX_LINE_LENGTH) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, sizeof(temp_line) - 1);
    temp_line[sizeof(temp_line) - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void process_logs(LogEntry entries[], int count, LogProcessor processor) {
    if (entries == NULL || processor == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        processor(&entries[i]);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    const char* sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:35:23 WARNING High memory usage detected",
        "2024-01-15T10:40:15 ERROR Database connection failed",
        "2024-01-15T10:45:30 INFO Backup completed",
        "2024-01-15T10:50:12 ERROR Authentication failed",
        "2024-01-15T10:55:47 WARNING Disk space running low",
        "2024-01-15T11:00:00 INFO Daily report generated",
        NULL
    };
    
    for (int i = 0; sample_logs[i] != NULL && entry_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &entries[entry_count])) {
            entry_count++;
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 1;
    }
    
    LogProcessor processors[] = {count_errors, print_warnings, find_keyword};
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    for (int i = 0; i < num_processors; i++) {
        printf("Processing with function %d:\n", i + 1);
        process_logs(entries, entry_count, processors[i]);
        printf("\n");
    }
    
    return 0;
}