//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

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
    
    entry->error_count = 0;
    if (strcmp(level, "ERROR") == 0) entry->error_count = 1;
    
    return 1;
}

int is_valid_timestamp(const char* timestamp) {
    if (timestamp == NULL) return 0;
    if (strlen(timestamp) != 19) return 0;
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    
    return 1;
}

void analyze_logs(LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int total_errors = 0;
    int total_warnings = 0;
    int total_info = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            total_errors++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            total_warnings++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            total_info++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", count);
    printf("INFO messages: %d\n", total_info);
    printf("WARNING messages: %d\n", total_warnings);
    printf("ERROR messages: %d\n", total_errors);
    
    if (count > 0) {
        printf("Error rate: %.2f%%\n", (total_errors * 100.0) / count);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    const char* sample_logs[] = {
        "2024-01-15 10:30:45 INFO System started successfully",
        "2024-01-15 10:31:22 WARNING High memory usage detected",
        "2024-01-15 10:32:10 ERROR Database connection failed",
        "2024-01-15 10:33:05 INFO User login successful",
        "2024-01-15 10:34:30 ERROR File not found",
        "2024-01-15 10:35:12 INFO Backup completed",
        "2024-01-15 10:36:45 WARNING Disk space low",
        "2024-01-15 10:37:20 INFO Cache cleared",
        "2024-01-15 10:38:15 ERROR Network timeout",
        "2024-01-15 10:39:00 INFO System shutdown initiated"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && entry_count < MAX_ENTRIES; i++) {
        LogEntry entry;
        if (parse_log_line(sample_logs[i], &entry)) {
            if (is_valid_timestamp(entry.timestamp)) {
                entries[entry_count] = entry;
                entry_count++;
            }
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count);
    
    printf("\nDetailed log entries:\n");
    for (int i = 0; i < entry_count && i < 5; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    if (entry_count > 5) {
        printf("... and %d more entries\n", entry_count - 5);
    }
    
    return 0;
}