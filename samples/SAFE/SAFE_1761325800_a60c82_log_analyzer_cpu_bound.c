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
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->error_count = 1;
    
    return 1;
}

int compare_entries(const void* a, const void* b) {
    const LogEntry* entry_a = (const LogEntry*)a;
    const LogEntry* entry_b = (const LogEntry*)b;
    return strcmp(entry_a->timestamp, entry_b->timestamp);
}

void analyze_log_patterns(LogEntry* entries, int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    char last_hour[8] = "";
    int hour_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].level, "ERROR") != NULL) error_count++;
        else if (strstr(entries[i].level, "WARN") != NULL) warning_count++;
        else if (strstr(entries[i].level, "INFO") != NULL) info_count++;
        
        char current_hour[8];
        strncpy(current_hour, entries[i].timestamp, 7);
        current_hour[7] = '\0';
        
        if (strcmp(current_hour, last_hour) != 0) {
            if (hour_count > 0) {
                printf("Hour %s: %d entries\n", last_hour, hour_count);
            }
            strcpy(last_hour, current_hour);
            hour_count = 1;
        } else {
            hour_count++;
        }
    }
    
    if (hour_count > 0) {
        printf("Hour %s: %d entries\n", last_hour, hour_count);
    }
    
    printf("\nSummary:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Total entries processed: %d\n", count);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Processing simulated log data...\n");
    
    const char* sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:31:15 WARN High memory usage detected",
        "2024-01-15T10:32:00 ERROR Database connection failed",
        "2024-01-15T10:33:45 INFO User login successful",
        "2024-01-15T10:34:20 ERROR File not found",
        "2024-01-15T10:35:10 INFO Backup completed",
        "2024-01-15T10:36:30 WARN Slow response time",
        "2024-01-15T10:37:05 ERROR Permission denied",
        "2024-01-15T10:38:00 INFO Cache cleared",
        "2024-01-15T10:39:15 WARN Disk space low"
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
    
    printf("Processing %d log entries...\n\n", entry_count);
    analyze_log_patterns(entries, entry_count);
    
    printf("\nTop 3 most frequent error messages:\n");
    int printed = 0;
    for (int i = 0; i < entry_count && printed < 3; i++) {
        if (strstr(entries[i].level, "ERROR") != NULL) {
            printf("%d. %s\n", printed + 1, entries[i].message);
            printed++;
        }
    }
    
    return 0;
}