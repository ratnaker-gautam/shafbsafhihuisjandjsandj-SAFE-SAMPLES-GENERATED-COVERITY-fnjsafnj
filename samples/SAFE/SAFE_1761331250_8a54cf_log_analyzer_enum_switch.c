//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

struct LogEntry {
    time_t timestamp;
    enum LogLevel level;
    char message[256];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "CRITICAL") == 0) return LOG_CRITICAL;
    return -1;
}

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

int main(void) {
    struct LogEntry logs[100];
    int log_count = 0;
    char input[512];
    
    printf("Log Analyzer - Enter log entries (format: LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'DONE' to finish input\n");
    
    while (log_count < 100) {
        printf("Log %d: ", log_count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "DONE") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        char level_str[32];
        char message[256];
        
        if (sscanf(input, "%31s %255[^\n]", level_str, message) != 2) {
            printf("Invalid format. Use: LEVEL MESSAGE\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Unknown log level: %s\n", level_str);
            continue;
        }
        
        if (strlen(message) >= 256) {
            printf("Message too long\n");
            continue;
        }
        
        logs[log_count].timestamp = time(NULL);
        logs[log_count].level = level;
        strncpy(logs[log_count].message, message, 255);
        logs[log_count].message[255] = '\0';
        log_count++;
    }
    
    if (log_count == 0) {
        printf("No logs to analyze\n");
        return 0;
    }
    
    printf("\n=== Log Analysis Results ===\n");
    
    int level_counts[4] = {0};
    for (int i = 0; i < log_count; i++) {
        level_counts[logs[i].level]++;
    }
    
    printf("Total logs: %d\n", log_count);
    printf("Breakdown by level:\n");
    
    for (enum LogLevel level = LOG_INFO; level <= LOG_CRITICAL; level++) {
        printf("  %s: %d\n", level_to_string(level), level_counts[level]);
    }
    
    printf("\nRecent logs:\n");
    int display_count = (log_count < 5) ? log_count : 5;
    for (int i = log_count - display_count; i < log_count; i++) {
        char time_buf[64];
        struct tm* timeinfo = localtime(&logs[i].timestamp);
        if (timeinfo != NULL) {
            strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", timeinfo);
        } else {
            strcpy(time_buf, "Unknown time");
        }
        
        printf("[%s] %s: %s\n", time_buf, level_to_string(logs[i].level), logs[i].message);
    }
    
    enum LogLevel highest_level = LOG_INFO;
    for (int i = 0; i < log_count; i++) {
        if (logs[i].level > highest_level) {
            highest_level = logs[i].level;
        }
    }
    
    printf("\nHighest severity level: %s\n", level_to_string(highest_level));
    
    switch (highest_level) {
        case LOG_INFO:
            printf("System status: Normal operation\n");
            break;
        case LOG_WARNING:
            printf("System status: Minor issues detected\n");
            break;
        case LOG_ERROR:
            printf("System status: Errors present, review needed\n");
            break;
        case LOG_CRITICAL:
            printf("System status: Critical failure, immediate action required\n");
            break;
        default:
            printf("System status: Unknown\n");
            break;
    }
    
    return 0;
}