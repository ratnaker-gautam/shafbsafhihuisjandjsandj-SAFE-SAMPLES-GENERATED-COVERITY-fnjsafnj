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
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' to finish input\n");
    printf("Maximum 100 log entries\n\n");
    
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
        char level_str[20];
        char message[256];
        
        int parsed = sscanf(input, "%19s %19s %255[^\n]", timestamp, level_str, message);
        
        if (parsed != 3) {
            printf("Invalid format. Use: YYYY-MM-DD HH:MM:SS LEVEL message\n");
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            printf("Invalid timestamp format. Use: YYYY-MM-DD HH:MM:SS\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level. Use: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
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
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total log entries: %d\n\n", log_count);
    
    printf("Log level distribution:\n");
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        printf("%-8s: %d entries\n", level_to_string(level), level_counts[level]);
    }
    
    printf("\nFilter logs by level (enter level name or 'ALL'): ");
    char filter[20];
    if (fgets(filter, sizeof(filter), stdin) != NULL) {
        filter[strcspn(filter, "\n")] = 0;
        
        printf("\nMatching log entries:\n");
        int displayed = 0;
        
        for (int i = 0; i < log_count; i++) {
            int should_display = 0;
            
            if (strcmp(filter, "ALL") == 0) {
                should_display = 1;
            } else {
                int filter_level = parse_log_level(filter);
                if (filter_level != -1 && logs[i].level == filter_level) {
                    should_display = 1;
                }
            }
            
            if (should_display) {
                printf("[%s] %-8s: %s\n", 
                       logs[i].timestamp, 
                       level_to_string(logs[i].level), 
                       logs[i].message);
                displayed++;
            }
        }
        
        if (displayed == 0) {
            printf("No matching log entries found.\n");
        }
    }
    
    return 0;
}