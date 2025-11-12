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
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* parts[3];
    char* token = strtok(temp_line, "|");
    int part_count = 0;
    
    while (token != NULL && part_count < 3) {
        parts[part_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
    if (strlen(parts[0]) >= sizeof(entry->timestamp) ||
        strlen(parts[1]) >= sizeof(entry->level) ||
        strlen(parts[2]) >= sizeof(entry->message)) {
        return 0;
    }
    
    strncpy(entry->timestamp, parts[0], sizeof(entry->timestamp) - 1);
    strncpy(entry->level, parts[1], sizeof(entry->level) - 1);
    strncpy(entry->message, parts[2], sizeof(entry->message) - 1);
    
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    entry->level[sizeof(entry->level) - 1] = '\0';
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int count_log_level(const LogEntry* entries, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

void analyze_log_patterns(const LogEntry* entries, int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].message, "error") != NULL ||
            strstr(entries[i].message, "failed") != NULL) {
            error_count++;
        } else if (strstr(entries[i].message, "warning") != NULL) {
            warning_count++;
        } else if (strstr(entries[i].message, "info") != NULL) {
            info_count++;
        }
    }
    
    printf("Pattern Analysis:\n");
    printf("  Messages containing 'error' or 'failed': %d\n", error_count);
    printf("  Messages containing 'warning': %d\n", warning_count);
    printf("  Messages containing 'info': %d\n", info_count);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    const char* sample_logs[] = {
        "2024-01-15 10:30:00|INFO|System startup completed",
        "2024-01-15 10:31:15|WARNING|High memory usage detected",
        "2024-01-15 10:32:00|ERROR|Database connection failed",
        "2024-01-15 10:33:45|INFO|User login successful",
        "2024-01-15 10:34:20|ERROR|File not found",
        "2024-01-15 10:35:10|WARNING|Disk space low",
        "2024-01-15 10:36:00|INFO|Backup completed",
        "2024-01-15 10:37:30|ERROR|Network timeout",
        "2024-01-15 10:38:15|INFO|Cache cleared",
        "2024-01-15 10:39:00|WARNING|CPU temperature high"
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
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    
    int info_count = count_log_level(entries, entry_count, "INFO");
    int warning_count = count_log_level(entries, entry_count, "WARNING");
    int error_count = count_log_level(entries, entry_count, "ERROR");
    
    printf("INFO entries: %d\n", info_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("ERROR entries: %d\n", error_count);
    
    printf("Distribution:\n");
    printf("  INFO: %.1f%%\n", (info_count * 100.0) / entry_count);
    printf("