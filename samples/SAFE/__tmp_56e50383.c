//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[64];
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

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) >= MAX_LINE_LENGTH - 1) {
            continue;
        }
        
        char timestamp[64] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%63s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) {
            continue;
        }
        
        if (!is_valid_timestamp(timestamp)) {
            continue;
        }
        
        int severity = parse_log_level(level);
        if (severity == -1) {
            continue;
        }
        
        if (strlen(message) == 0) {
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

void analyze_logs(LogEntry entries[], int count) {
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
        printf("%-8s: %d entries\n", level_names[i], level_counts[i]);
    }
    
    if (count > 0) {
        double avg_severity = (double)total_severity / count;
        printf("\nAverage severity: %.2f\n", avg_severity);
        
        int error_critical = level_counts[3] + level_counts[4];
        double error_percentage = (double)error_critical * 100.0 / count;
        printf("Error/Critical percentage: %.1f%%\n", error_percentage);
    }
    
    printf("\nMost frequent messages:\n");
    int message_counts[100] = {0};
    char* unique_messages[100] = {0};
    int unique_count = 0;
    
    for (int i = 0; i < count && unique_count < 100; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(entries[i].message, unique_messages[j]) == 0) {
                message_counts[j]++;
                found = 1;
                break;
            }
        }
        if (!found) {
            unique_messages[unique_count] = entries[i].message;
            message_counts[unique_count] = 1;
            unique_count++;
        }
    }
    
    for (int i = 0; i < 5 && i < unique_count; i++) {
        int max_idx = i;
        for (int j = i + 1; j < unique_count; j++) {
            if (message_counts[j] > message_counts[max_idx]) {
                max_idx = j;
            }
        }
        if (max_idx != i) {
            int temp_count = message