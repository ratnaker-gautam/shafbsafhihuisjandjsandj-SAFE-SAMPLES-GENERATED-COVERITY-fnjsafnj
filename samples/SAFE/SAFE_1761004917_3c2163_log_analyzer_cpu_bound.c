//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
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
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* parts[3];
    int part_count = 0;
    char* token = strtok(temp, "|");
    
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

int count_log_level(const LogEntry* logs, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

void analyze_log_patterns(const LogEntry* logs, int count) {
    int error_count = count_log_level(logs, count, "ERROR");
    int warning_count = count_log_level(logs, count, "WARNING");
    int info_count = count_log_level(logs, count, "INFO");
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (count > 0) {
        printf("Error rate: %.2f%%\n", (error_count * 100.0) / count);
        printf("Warning rate: %.2f%%\n", (warning_count * 100.0) / count);
    }
}

int main() {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    const char* sample_logs[] = {
        "2024-01-15 10:30:25|INFO|System startup completed",
        "2024-01-15 10:31:10|WARNING|High memory usage detected",
        "2024-01-15 10:32:45|ERROR|Database connection failed",
        "2024-01-15 10:33:20|INFO|User login successful",
        "2024-01-15 10:34:05|ERROR|File not found",
        "2024-01-15 10:35:30|WARNING|CPU temperature high",
        "2024-01-15 10:36:15|INFO|Backup completed",
        "2024-01-15 10:37:40|ERROR|Network timeout",
        "2024-01-15 10:38:25|INFO|Configuration updated",
        "2024-01-15 10:39:10|WARNING|Disk space low"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && log_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &logs[log_count])) {
            log_count++;
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_log_patterns(logs, log_count);
    
    printf("\nDetailed log entries:\n");
    for (int i = 0; i < log_count; i++) {
        printf("%s | %s | %s\n", 
               logs[i].timestamp, 
               logs[i].level, 
               logs[i].message);
    }
    
    return 0;
}