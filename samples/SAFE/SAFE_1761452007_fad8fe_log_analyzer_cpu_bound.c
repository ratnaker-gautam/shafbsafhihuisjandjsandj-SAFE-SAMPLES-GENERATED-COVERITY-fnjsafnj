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
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp, line);
    
    char* token = strtok(temp, " ");
    if (!token) return 0;
    if (strlen(token) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, token);
    
    token = strtok(NULL, " ");
    if (!token) return 0;
    if (strlen(token) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, token);
    
    token = strtok(NULL, "\n");
    if (!token) return 0;
    if (strlen(token) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, token);
    
    entry->error_count = 0;
    if (strcmp(entry->level, "ERROR") == 0) {
        entry->error_count = 1;
    }
    
    return 1;
}

int is_valid_timestamp(const char* timestamp) {
    if (!timestamp || strlen(timestamp) != 19) return 0;
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    char line[MAX_LINE_LEN];
    
    printf("Log Analyzer - Processing synthetic log data...\n");
    
    const char* sample_logs[] = {
        "2023-10-15 08:30:15 INFO System startup completed",
        "2023-10-15 08:31:22 WARN Low disk space on /var",
        "2023-10-15 08:32:45 ERROR Database connection failed",
        "2023-10-15 08:33:10 INFO User login successful",
        "2023-10-15 08:34:55 ERROR File not found: config.xml",
        "2023-10-15 08:35:20 INFO Backup process started",
        "2023-10-15 08:36:30 WARN High memory usage detected",
        "2023-10-15 08:37:05 INFO Cache cleared successfully",
        "2023-10-15 08:38:40 ERROR Network timeout occurred",
        "2023-10-15 08:39:15 INFO Task completed normally"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && entry_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &entries[entry_count])) {
            if (is_valid_timestamp(entries[entry_count].timestamp)) {
                total_errors += entries[entry_count].error_count;
                entry_count++;
            }
        }
    }
    
    printf("Processed %d log entries\n", entry_count);
    printf("Total errors found: %d\n", total_errors);
    
    printf("\nError Summary:\n");
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].error_count > 0) {
            printf("ERROR: %s - %s\n", entries[i].timestamp, entries[i].message);
        }
    }
    
    int info_count = 0;
    int warn_count = 0;
    int error_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(entries[i].level, "WARN") == 0) {
            warn_count++;
        } else if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        }
    }
    
    printf("\nLevel Distribution:\n");
    printf("INFO: %d entries\n", info_count);
    printf("WARN: %d entries\n", warn_count);
    printf("ERROR: %d entries\n", error_count);
    
    if (entry_count > 0) {
        double error_rate = (double)error_count / entry_count * 100.0;
        printf("Error rate: %.2f%%\n", error_rate);
    }
    
    return 0;
}