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
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    if (!timestamp) return 0;
    
    char* level = strtok(NULL, " ");
    if (!level) return 0;
    
    char* message = strtok(NULL, "\n");
    if (!message) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || 
           strcmp(level, "WARN") == 0 || 
           strcmp(level, "ERROR") == 0;
}

void generate_sample_logs(LogEntry* logs, int count) {
    const char* levels[] = {"INFO", "WARN", "ERROR"};
    const char* messages[] = {
        "System started successfully",
        "User login attempt",
        "Database connection established",
        "Memory usage high",
        "Disk space low",
        "Network timeout",
        "Invalid input detected",
        "Backup completed",
        "Security alert triggered",
        "Configuration updated"
    };
    
    time_t base_time = time(NULL);
    
    for (int i = 0; i < count; i++) {
        int level_idx = rand() % 3;
        int msg_idx = rand() % 10;
        
        time_t log_time = base_time - (rand() % 86400);
        struct tm* tm_info = localtime(&log_time);
        
        strftime(logs[i].timestamp, sizeof(logs[i].timestamp), 
                "%Y-%m-%d %H:%M:%S", tm_info);
        strncpy(logs[i].level, levels[level_idx], sizeof(logs[i].level) - 1);
        logs[i].level[sizeof(logs[i].level) - 1] = '\0';
        strncpy(logs[i].message, messages[msg_idx], sizeof(logs[i].message) - 1);
        logs[i].message[sizeof(logs[i].message) - 1] = '\0';
    }
}

void analyze_logs(LogEntry* logs, int count) {
    int info_count = 0, warn_count = 0, error_count = 0;
    char recent_timestamp[32] = "";
    char oldest_timestamp[32] = "";
    
    if (count > 0) {
        strncpy(recent_timestamp, logs[0].timestamp, sizeof(recent_timestamp) - 1);
        strncpy(oldest_timestamp, logs[0].timestamp, sizeof(oldest_timestamp) - 1);
    }
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) info_count++;
        else if (strcmp(logs[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(logs[i].level, "ERROR") == 0) error_count++;
        
        if (strcmp(logs[i].timestamp, recent_timestamp) > 0) {
            strncpy(recent_timestamp, logs[i].timestamp, sizeof(recent_timestamp) - 1);
        }
        if (strcmp(logs[i].timestamp, oldest_timestamp) < 0) {
            strncpy(oldest_timestamp, logs[i].timestamp, sizeof(oldest_timestamp) - 1);
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("Time range: %s to %s\n", oldest_timestamp, recent_timestamp);
    
    double info_percent = count > 0 ? (info_count * 100.0) / count : 0;
    double warn_percent = count > 0 ? (warn_count * 100.0) / count : 0;
    double error_percent = count > 0 ? (error_count * 100.0) / count : 0;
    
    printf("Distribution: INFO %.1f%%, WARN %.1f%%, ERROR %.1f%%\n", 
           info_percent, warn_percent, error_percent);
}

int