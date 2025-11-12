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
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    char line[MAX_LINE_LEN];
    
    printf("Log Analyzer - Processing simulated log data...\n");
    
    const char* sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:31:15 WARN High memory usage detected",
        "2024-01-15T10:32:00 ERROR Database connection failed",
        "2024-01-15T10:33:45 INFO Backup completed",
        "2024-01-15T10:34:20 ERROR File not found: config.xml",
        "2024-01-15T10:35:00 INFO User login successful",
        "2024-01-15T10:36:30 WARN Slow response time",
        "2024-01-15T10:37:10 ERROR Permission denied",
        "2024-01-15T10:38:00 INFO Cache cleared",
        "2024-01-15T10:39:25 INFO System shutdown initiated"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && entry_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &entries[entry_count])) {
            total_errors += entries[entry_count].error_count;
            entry_count++;
        }
    }
    
    printf("Processed %d log entries\n", entry_count);
    printf("Total errors found: %d\n", total_errors);
    
    if (entry_count > 0) {
        printf("\nError Summary:\n");
        for (int i = 0; i < entry_count; i++) {
            if (entries[i].error_count > 0) {
                printf("ERROR: %s - %s\n", entries[i].timestamp, entries[i].message);
            }
        }
        
        int info_count = 0;
        int warn_count = 0;
        
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(entries[i].level, "INFO") == 0) {
                info_count++;
            } else if (strcmp(entries[i].level, "WARN") == 0) {
                warn_count++;
            }
        }
        
        printf("\nLog Level Distribution:\n");
        printf("INFO: %d entries\n", info_count);
        printf("WARN: %d entries\n", warn_count);
        printf("ERROR: %d entries\n", total_errors);
        
        if (entry_count > 0) {
            double error_rate = (double)total_errors / entry_count * 100.0;
            printf("Error rate: %.2f%%\n", error_rate);
        }
    }
    
    printf("\nAnalysis complete.\n");
    return 0;
}