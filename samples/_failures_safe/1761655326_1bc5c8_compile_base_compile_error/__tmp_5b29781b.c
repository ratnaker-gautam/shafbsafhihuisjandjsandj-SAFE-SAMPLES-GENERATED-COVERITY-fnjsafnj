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
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    if (timestamp == NULL) return 0;
    
    char* level = strtok(NULL, " ");
    if (level == NULL) return 0;
    
    char* message = strtok(NULL, "\n");
    if (message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || 
           strcmp(level, "WARN") == 0 || 
           strcmp(level, "ERROR") == 0;
}

void analyze_logs(LogEntry* entries, int count) {
    int info_count = 0, warn_count = 0, error_count = 0;
    char last_timestamp[32] = "";
    int total_messages = 0;
    
    for (int i = 0; i < count; i++) {
        if (!is_valid_level(entries[i].level)) continue;
        
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        
        total_messages++;
        strcpy(last_timestamp, entries[i].timestamp);
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", total_messages);
    printf("INFO level entries: %d\n", info_count);
    printf("WARN level entries: %d\n", warn_count);
    printf("ERROR level entries: %d\n", error_count);
    printf("Last timestamp: %s\n", last_timestamp);
    
    if (total_messages > 0) {
        printf("INFO percentage: %.2f%%\n", (info_count * 100.0) / total_messages);
        printf("WARN percentage: %.2f%%\n", (warn_count * 100.0) / total_messages);
        printf("ERROR percentage: %.2f%%\n", (error_count * 100.0) / total_messages);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    const char* sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:31:15 WARN High memory usage detected",
        "2024-01-15T10:32:00 INFO User login successful",
        "2024-01-15T10:33:45 ERROR Database connection failed",
        "2024-01-15T10:34:10 INFO Backup completed",
        "2024-01-15T10:35:20 WARN Disk space running low",
        "2024-01-15T10:36:05 INFO New user registered",
        "2024-01-15T10:37:30 ERROR Payment processing failed",
        "2024-01-15T10:38:15 INFO Cache cleared",
        "2024-01-15T10:39:00 WARN Network latency detected"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && entry_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &entries[entry_count])) {
            entry_count++;
        }
    }
    
    printf("Processing %d log entries...\n", entry_count);
    
    clock_t start_time = clock();
    
    for (int iter = 0; iter < 10000; iter++) {
        for (int i = 0; i < entry_count; i++) {
            for (int j = 0; j < 10; j++) {
                char temp[256];
                strncpy(temp, entries[i].message, sizeof(temp) - 1);
                temp[sizeof(temp) - 1] = '\0';
            }
        }
    }
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    analyze_logs(entries, entry_count);
    printf("CPU processing time: %.4f seconds\n", cpu_time);
    return