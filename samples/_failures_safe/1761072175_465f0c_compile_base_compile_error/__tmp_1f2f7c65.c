//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
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
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    entry->error_count = 0;
    if (strcmp(level, "ERROR") == 0) {
        entry->error_count = 1;
    }
    
    return 1;
}

int validate_timestamp(const char* timestamp) {
    if (timestamp == NULL) return 0;
    
    int year, month, day, hour, min, sec;
    if (sscanf(timestamp, "%d-%d-%dT%d:%d:%d", &year, &month, &day, &hour, &min, &sec) != 6) {
        return 0;
    }
    
    if (year < 2000 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (hour < 0 || hour > 23) return 0;
    if (min < 0 || min > 59) return 0;
    if (sec < 0 || sec > 59) return 0;
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Processing simulated log data...\n");
    
    const char* sample_logs[] = {
        "2024-03-15T10:30:45 INFO System started successfully",
        "2024-03-15T10:31:22 WARN Low memory detected",
        "2024-03-15T10:32:15 ERROR Database connection failed",
        "2024-03-15T10:33:01 INFO User login successful",
        "2024-03-15T10:33:45 ERROR File not found",
        "2024-03-15T10:34:12 INFO Backup completed",
        "2024-03-15T10:35:03 WARN High CPU usage",
        "2024-03-15T10:35:47 ERROR Network timeout",
        "2024-03-15T10:36:22 INFO Cache cleared",
        "2024-03-15T10:37:05 INFO System shutdown initiated"
    };
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && entry_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &entries[entry_count])) {
            if (validate_timestamp(entries[entry_count].timestamp)) {
                total_errors += entries[entry_count].error_count;
                entry_count++;
            }
        }
    }
    
    clock_t start_time = clock();
    
    for (int i = 0; i < entry_count; i++) {
        for (int j = 0; j < entry_count - 1; j++) {
            if (strcmp(entries[j].timestamp, entries[j + 1].timestamp) > 0) {
                LogEntry temp = entries[j];
                entries[j] = entries[j + 1];
                entries[j + 1] = temp;
            }
        }
    }
    
    int info_count = 0;
    int warn_count = 0;
    int error_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
    }
    
    clock_t end_time = clock();
    double processing_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("\nAnalysis Results:\n");
    printf("Total log entries: %d\n", entry_count);
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);