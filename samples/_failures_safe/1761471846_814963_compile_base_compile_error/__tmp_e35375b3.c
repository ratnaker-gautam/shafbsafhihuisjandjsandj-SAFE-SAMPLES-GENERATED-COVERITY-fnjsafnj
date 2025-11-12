//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
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
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    int total_warnings = 0;
    
    printf("Log Analyzer - Processing simulated log data...\n");
    
    const char* sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:31:15 WARNING High memory usage detected",
        "2024-01-15T10:32:00 ERROR Database connection failed",
        "2024-01-15T10:33:45 INFO Backup completed",
        "2024-01-15T10:34:20 ERROR File not found: config.xml",
        "2024-01-15T10:35:10 WARNING CPU temperature above threshold",
        "2024-01-15T10:36:30 INFO User login: john_doe",
        "2024-01-15T10:37:05 ERROR Permission denied",
        "2024-01-15T10:38:00 INFO Cache cleared",
        "2024-01-15T10:39:15 WARNING Network latency detected"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && entry_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &entries[entry_count])) {
            if (strcmp(entries[entry_count].level, "ERROR") == 0) {
                total_errors++;
            } else if (strcmp(entries[entry_count].level, "WARNING") == 0) {
                total_warnings++;
            }
            entry_count++;
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
    
    char most_common_level[16] = "INFO";
    int level_counts[3] = {0};
    const char* levels[] = {"INFO", "WARNING", "ERROR"};
    
    for (int i = 0; i < entry_count; i++) {
        for (int j = 0; j < 3; j++) {
            if (strcmp(entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    int max_count = level_counts[0];
    strncpy(most_common_level, levels[0], 15);
    most_common_level[15] = '\0';
    for (int i = 1; i < 3; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            strncpy(most_common_level, levels[i], 15);
            most_common_level[15] = '\0';
        }
    }
    
    clock_t end_time = clock();
    double processing_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("Analysis Results:\n");
    printf("Total log entries: %d\n", entry_count);
    printf("Errors: %d\n", total_errors);
    printf("Warnings: %d\n", total_warnings);
    printf("Most common log level: %s\n", most_common_level);
    printf("Processing time: %.6f seconds\n", processing_time);
    
    printf("\nFirst 5 sorted entries:\n");
    int display_count = (entry_count < 5) ? entry_count : 5;
    for (int i = 0; i < display_count; i++) {
        printf("%s