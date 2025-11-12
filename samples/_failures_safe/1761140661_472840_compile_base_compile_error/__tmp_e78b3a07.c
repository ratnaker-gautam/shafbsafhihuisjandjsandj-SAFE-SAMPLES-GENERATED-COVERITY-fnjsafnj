//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

struct LogEntry {
    char timestamp[20];
    enum LogLevel level;
    char message[256];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return LOG_DEBUG;
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "CRITICAL") == 0) return LOG_CRITICAL;
    return -1;
}

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_DEBUG: return "DEBUG";
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
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

int main(void) {
    struct LogEntry logs[100];
    int log_count = 0;
    int level_counts[5] = {0};
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE)\n");
    printf("Enter 'END' to finish input\n");
    
    char input[512];
    while (log_count < 100) {
        printf("Log entry %d: ", log_count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "END") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        char timestamp[20];
        char level_str[16];
        char message[256];
        
        int parsed = sscanf(input, "%19s %15s %255[^\n]", timestamp, level_str, message);
        
        if (parsed != 3) {
            printf("Invalid format. Use: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE\n");
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            printf("Invalid timestamp format: %s\n", timestamp);
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level: %s\n", level_str);
            continue;
        }
        
        if (strlen(message) == 0) {
            printf("Message cannot be empty\n");
            continue;
        }
        
        strcpy(logs[log_count].timestamp, timestamp);
        logs[log_count].level = level;
        strcpy(logs[log_count].message, message);
        
        level_counts[level]++;
        log_count++;
    }
    
    printf("\n=== Log Analysis Report ===\n");
    printf("Total entries: %d\n", log_count);
    printf("\nBreakdown by level:\n");
    
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d entries\n", level_to_string(i), level_counts[i]);
    }
    
    if (log_count > 0) {
        printf("\nRecent entries:\n");
        int display_count = (log_count < 5) ? log_count : 5;
        for (int i = log_count - display_count; i < log_count; i++) {
            printf("[%s] %-8s: %s\n", 
                   logs[i].timestamp, 
                   level_to_string(logs[i].level), 
                   logs[i].message);
        }
        
        enum LogLevel highest_level = LOG_DEBUG;
        for (int i = 0; i < log_count; i++) {
            if (logs[i].level > highest_level) {
                highest_level = logs[i].level;
            }
        }
        
        printf("\nHighest severity level found: %s\n", level_to_string(highest_level));
        
        switch (highest_level) {
            case LOG_CRITICAL:
                printf("ALERT: Critical issues detected!\n");
                break;
            case LOG_ERROR:
                printf("Warning: Error conditions present\n");
                break;
            case LOG_WARNING:
                printf("Note: Warnings detected\n");
                break;
            case LOG_INFO:
                printf("System operating normally\n");
                break;
            case LOG_DEBUG:
                printf("Only