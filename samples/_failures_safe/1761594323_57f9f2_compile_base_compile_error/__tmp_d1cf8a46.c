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
    int severity;
} LogEntry;

int parse_log_level(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int validate_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
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

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (result != 3) return 0;
    
    if (!validate_timestamp(timestamp)) return 0;
    
    int severity = parse_log_level(level);
    if (severity == -1) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    strncpy(entry->level, level, 15);
    strncpy(entry->message, message, 255);
    entry->severity = severity;
    
    return 1;
}

void analyze_logs(LogEntry* logs, int count) {
    if (logs == NULL || count <= 0) return;
    
    int severity_count[5] = {0};
    int total_messages = 0;
    
    for (int i = 0; i < count; i++) {
        if (logs[i].severity >= 0 && logs[i].severity < 5) {
            severity_count[logs[i].severity]++;
            total_messages++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", total_messages);
    printf("DEBUG: %d\n", severity_count[0]);
    printf("INFO: %d\n", severity_count[1]);
    printf("WARNING: %d\n", severity_count[2]);
    printf("ERROR: %d\n", severity_count[3]);
    printf("CRITICAL: %d\n", severity_count[4]);
    
    if (total_messages > 0) {
        printf("Error rate: %.2f%%\n", 
               (float)(severity_count[3] + severity_count[4]) * 100.0 / total_messages);
    }
}

int compare_by_severity(const void* a, const void* b) {
    const LogEntry* log_a = (const LogEntry*)a;
    const LogEntry* log_b = (const LogEntry*)b;
    return log_b->severity - log_a->severity;
}

void generate_sample_logs(LogEntry* logs, int count) {
    if (logs == NULL || count <= 0) return;
    
    const char* levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    const char* messages[] = {
        "System startup completed",
        "User login successful",
        "Database connection established",
        "Memory usage high",
        "Disk space running low",
        "Network timeout occurred",
        "Authentication failed",
        "Service unavailable",
        "Configuration error detected",
        "Backup process started"
    };
    
    time_t current_time = time(NULL);
    struct tm time_info;
    localtime_r(&current_time, &time_info);
    
    for (int i = 0; i < count; i++) {
        int level_idx = rand() % 5;
        int msg_idx = rand() % 10;
        
        time_info.tm_sec = (time_info.tm_sec + i) % 60;
        time_info.tm_min = (time_info.tm_min + i) % 60;
        time_info.tm_hour = (time_info.tm_hour + i) % 24;
        
        snprintf(logs[i].timestamp, 32, "%04d-%02d-%02d %02d:%02d:%02d",
                 time_info.tm_year + 1900, time_info.tm_mon + 1, time_info.tm_mday,
                 time_info.tm_hour, time_info.tm_min, time_info.tm_sec);
        
        strncpy(logs