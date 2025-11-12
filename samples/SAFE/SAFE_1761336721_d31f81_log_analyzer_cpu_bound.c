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

int is_valid_timestamp(const char* timestamp) {
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

int read_log_entries(LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) >= sizeof(line) - 1) {
            continue;
        }
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) {
            continue;
        }
        
        if (!is_valid_timestamp(timestamp)) {
            continue;
        }
        
        int severity = parse_log_level(level);
        if (severity == 0) {
            continue;
        }
        
        strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].severity = severity;
        count++;
    }
    
    return count;
}

void analyze_logs(LogEntry* entries, int count) {
    int severity_count[6] = {0};
    int total_severity = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 1 && entries[i].severity <= 5) {
            severity_count[entries[i].severity]++;
            total_severity += entries[i].severity;
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
    
    int error_critical_count = severity_count[4] + severity_count[5];
    printf("Error+Critical entries: %d\n", error_critical_count);
    
    if (count > 0) {
        double error_percentage = (double)error_critical_count * 100.0 / count;
        printf("Error percentage: %.1f%%\n", error_percentage);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE):\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to end input.\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(entries, count);
    
    return 0;
}