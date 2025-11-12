//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
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
    char line[MAX_LINE_LEN];
    
    const char* sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:31:15 WARNING High memory usage detected",
        "2024-01-15T10:32:00 ERROR Database connection failed",
        "2024-01-15T10:33:45 INFO User login successful",
        "2024-01-15T10:34:20 ERROR File not found",
        "2024-01-15T10:35:00 WARNING Slow response time",
        "2024-01-15T10:36:10 INFO Backup completed",
        "2024-01-15T10:37:30 ERROR Permission denied",
        "2024-01-15T10:38:00 INFO Cache cleared",
        "2024-01-15T10:39:15 WARNING Disk space low"
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
    
    printf("Log Analysis Results:\n");
    printf("====================\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Total errors: %d\n", total_errors);
    printf("Total warnings: %d\n", total_warnings);
    printf("Error rate: %.2f%%\n", entry_count > 0 ? (total_errors * 100.0) / entry_count : 0.0);
    
    printf("\nError entries:\n");
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            printf("- %s: %s\n", entries[i].timestamp, entries[i].message);
        }
    }
    
    printf("\nWarning entries:\n");
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "WARNING") == 0) {
            printf("- %s: %s\n", entries[i].timestamp, entries[i].message);
        }
    }
    
    clock_t start_time = clock();
    
    for (int iter = 0; iter < 100000; iter++) {
        int temp_errors = 0;
        int temp_warnings = 0;
        
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                temp_errors++;
            } else if (strcmp(entries[i].level, "WARNING") == 0) {
                temp_warnings++;
            }
        }
    }
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("\nCPU-intensive analysis completed in %.6f seconds\n", cpu_time);
    
    return 0;
}