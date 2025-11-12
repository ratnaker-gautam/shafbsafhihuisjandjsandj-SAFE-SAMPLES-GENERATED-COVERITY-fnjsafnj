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
    int severity;
} LogEntry;

int parse_log_level(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "CRITICAL") == 0) return 5;
    return 0;
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

int parse_log_entry(const char* line, LogEntry* entry) {
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    if (!timestamp || !validate_timestamp(timestamp)) return 0;
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    
    char* level = strtok(NULL, " ");
    if (!level) return 0;
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    
    entry->severity = parse_log_level(entry->level);
    if (entry->severity == 0) return 0;
    
    char* message = strtok(NULL, "\n");
    if (!message) return 0;
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    return 1;
}

void analyze_logs(LogEntry* logs, int count) {
    int severity_count[6] = {0};
    int total_severity = 0;
    
    for (int i = 0; i < count; i++) {
        if (logs[i].severity >= 1 && logs[i].severity <= 5) {
            severity_count[logs[i].severity]++;
            total_severity += logs[i].severity;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", count);
    printf("DEBUG: %d\n", severity_count[1]);
    printf("INFO: %d\n", severity_count[2]);
    printf("WARNING: %d\n", severity_count[3]);
    printf("ERROR: %d\n", severity_count[4]);
    printf("CRITICAL: %d\n", severity_count[5]);
    
    if (count > 0) {
        double avg_severity = (double)total_severity / count;
        printf("Average severity: %.2f\n", avg_severity);
    }
    
    int max_severity = 0;
    for (int i = 0; i < count; i++) {
        if (logs[i].severity > max_severity) {
            max_severity = logs[i].severity;
        }
    }
    printf("Highest severity level: %d\n", max_severity);
}

void generate_sample_logs(LogEntry* logs, int* count) {
    const char* levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    const char* messages[] = {
        "System startup completed",
        "User login successful",
        "Database connection established",
        "Memory usage at 75 percent",
        "Disk space running low",
        "Network latency detected",
        "Backup process started",
        "Security scan initiated",
        "Configuration updated",
        "Performance metrics collected"
    };
    
    time_t now = time(NULL);
    struct tm tm_info;
    localtime_r(&now, &tm_info);
    
    *count = 0;
    for (int i = 0; i < 50 && *count < MAX_ENTRIES; i++) {
        LogEntry entry;
        
        struct tm current_time = tm_info;
        current_time.tm_sec = (current_time.tm_sec + i) % 60;
        current_time.tm_min = (current_time.tm_min + i * 2) % 60;
        current_time.tm_hour = (current_time.tm_hour + i) % 24;
        
        strftime(entry.timestamp, 32, "%Y-%m-%d %H:%M:%S", &current_time);
        
        int level_idx = rand() % 5;
        strncpy(entry.level, levels[