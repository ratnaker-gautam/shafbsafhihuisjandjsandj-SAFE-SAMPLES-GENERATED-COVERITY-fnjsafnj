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
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* token = strtok(temp_line, " ");
    if (token == NULL) return 0;
    
    strncpy(entry->timestamp, token, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    token = strtok(NULL, " ");
    if (token == NULL) return 0;
    
    strncpy(entry->level, token, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    token = strtok(NULL, "\n");
    if (token == NULL) return 0;
    
    strncpy(entry->message, token, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    entry->error_count = 0;
    const char* ptr = entry->message;
    while (*ptr) {
        if (strncmp(ptr, "error", 5) == 0 && !isalnum(ptr[5])) {
            entry->error_count++;
        }
        ptr++;
    }
    
    return 1;
}

int compare_entries(const void* a, const void* b) {
    const LogEntry* entry_a = (const LogEntry*)a;
    const LogEntry* entry_b = (const LogEntry*)b;
    return strcmp(entry_a->timestamp, entry_b->timestamp);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Processing synthetic log data...\n");
    
    const char* sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:31:15 WARN High memory usage detected",
        "2024-01-15T10:32:45 ERROR Database connection failed",
        "2024-01-15T10:33:20 INFO Retrying connection",
        "2024-01-15T10:34:10 ERROR Authentication error in module",
        "2024-01-15T10:35:00 INFO Connection established",
        "2024-01-15T10:36:30 WARN Disk space running low",
        "2024-01-15T10:37:45 ERROR Critical system error occurred",
        "2024-01-15T10:38:20 INFO Backup process started",
        "2024-01-15T10:39:05 INFO System shutdown initiated"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && entry_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &entries[entry_count])) {
            entry_count++;
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    qsort(entries, entry_count, sizeof(LogEntry), compare_entries);
    
    int total_errors = 0;
    int total_warnings = 0;
    int total_info = 0;
    int total_error_mentions = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            total_errors++;
        } else if (strcmp(entries[i].level, "WARN") == 0) {
            total_warnings++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            total_info++;
        }
        total_error_mentions += entries[i].error_count;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("ERROR level entries: %d\n", total_errors);
    printf("WARN level entries: %d\n", total_warnings);
    printf("INFO level entries: %d\n", total_info);
    printf("Total 'error' mentions in messages: %d\n", total_error_mentions);
    
    printf("\nSorted log entries by timestamp:\n");
    for (int i = 0; i < entry_count && i < 5; i++) {
        printf("%s [%s] %s (error mentions: %d)\n", 
               entries[i].timestamp, entries[i].level, 
               entries[i].message, entries[i].error_count);
    }
    
    if (entry_count > 5) {
        printf("... and %d more entries\n", entry_count - 5);
    }
    
    clock_t