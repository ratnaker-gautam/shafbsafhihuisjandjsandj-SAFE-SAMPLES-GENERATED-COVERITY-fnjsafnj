//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
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

int parse_log_line(const char* line, LogEntry* entry) {
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    
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
    
    if (!validate_timestamp(parts[0])) return 0;
    strncpy(entry->timestamp, parts[0], 31);
    entry->timestamp[31] = '\0';
    
    strncpy(entry->level, parts[1], 15);
    entry->level[15] = '\0';
    
    strncpy(entry->message, parts[2], 255);
    entry->message[255] = '\0';
    
    entry->severity = parse_log_level(entry->level);
    if (entry->severity == 0) return 0;
    
    return 1;
}

void analyze_logs(LogEntry* logs, int count) {
    int level_counts[6] = {0};
    int total_severity = 0;
    
    for (int i = 0; i < count; i++) {
        if (logs[i].severity >= 1 && logs[i].severity <= 5) {
            level_counts[logs[i].severity]++;
            total_severity += logs[i].severity;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("DEBUG: %d\n", level_counts[1]);
    printf("INFO: %d\n", level_counts[2]);
    printf("WARNING: %d\n", level_counts[3]);
    printf("ERROR: %d\n", level_counts[4]);
    printf("CRITICAL: %d\n", level_counts[5]);
    
    if (count > 0) {
        double avg_severity = (double)total_severity / count;
        printf("Average severity: %.2f\n", avg_severity);
    }
}

int main(void) {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    const char* sample_logs[] = {
        "2024-01-15 10:30:25|INFO|User login successful",
        "2024-01-15 10:31:10|DEBUG|Session initialized",
        "2024-01-15 10:32:45|WARNING|High memory usage detected",
        "2024-01-15 10:33:20|ERROR|Database connection failed",
        "2024-01-15 10:34:05|INFO|Backup process started",
        "2024-01-15 10:35:30|CRITICAL|System crash imminent",
        "2024-01-15 10:36:15|INFO|Recovery process completed",
        "2024-01-15 10:37:00|DEBUG|Cache cleared",
        "2024-01-15 10:38:25|ERROR|File not found",
        "2024-01-15 10:39:10|WARNING|Disk space low"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && log_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &logs[log_count])) {
            log_count++;
        }
    }