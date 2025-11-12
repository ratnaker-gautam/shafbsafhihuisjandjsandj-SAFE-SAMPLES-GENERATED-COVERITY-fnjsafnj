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
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
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
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) continue;
        
        if (!is_valid_timestamp(timestamp)) continue;
        
        int severity = parse_log_level(level);
        if (severity == -1) continue;
        
        strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].severity = severity;
        count++;
    }
    
    return count;
}

void analyze_logs(LogEntry* entries, int count) {
    int level_counts[5] = {0};
    char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    int total_severity = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            level_counts[entries[i].severity]++;
            total_severity += entries[i].severity;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", count);
    printf("\nBreakdown by level:\n");
    
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d entries (%.1f%%)\n", 
               level_names[i], level_counts[i],
               count > 0 ? (level_counts[i] * 100.0) / count : 0.0);
    }
    
    printf("\nAverage severity: %.2f\n", 
           count > 0 ? (double)total_severity / count : 0.0);
    
    int error_critical_count = level_counts[3] + level_counts[4];
    printf("Error+Critical entries: %d\n", error_critical_count);
    
    if (count > 0) {
        printf("Error rate: %.1f%%\n", (error_critical_count * 100.0) / count);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Log Analyzer - Enter log entries (timestamp level message):\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL message text\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter empty line to finish input\n\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    if (count == MAX_ENTRIES) {
        printf("Warning: Reached maximum entry limit (%d)\n", MAX_ENTRIES);
    }
    
    analyze_logs(entries, count);
    
    return 0;
}