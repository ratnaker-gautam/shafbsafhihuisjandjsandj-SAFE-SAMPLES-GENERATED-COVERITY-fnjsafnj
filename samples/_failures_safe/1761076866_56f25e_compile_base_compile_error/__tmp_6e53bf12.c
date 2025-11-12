//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
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
    char buffer[512];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Example: 1609459200 INFO \"System started\"\n");
    printf("Enter 'done' to finish input\n");
    
    while (log_count < 100) {
        printf("Log %d: ", log_count + 1);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (strcmp(buffer, "done") == 0) {
            break;
        }
        
        if (strlen(buffer) == 0) {
            continue;
        }
        
        char timestamp_str[32];
        char level_str[16];
        char message[256];
        
        int parsed = sscanf(buffer, "%31s %15s %255[^\n]", 
                           timestamp_str, level_str, message);
        
        if (parsed != 3) {
            printf("Invalid format. Use: TIMESTAMP LEVEL MESSAGE\n");
            continue;
        }
        
        long timestamp = atol(timestamp_str);
        if (timestamp <= 0) {
            printf("Invalid timestamp\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level. Use: INFO, WARNING, ERROR, CRITICAL\n");
            continue;
        }
        
        if (strlen(message) == 0) {
            printf("Message cannot be empty\n");
            continue;
        }
        
        logs[log_count].timestamp = (time_t)timestamp;
        logs[log_count].level = (enum LogLevel)level;
        strncpy(logs[log_count].message, message, sizeof(logs[log_count].message) - 1);
        logs[log_count].message[sizeof(logs[log_count].message) - 1] = '\0';
        
        log_count++;
    }
    
    if (log_count == 0) {
        printf("No logs to analyze\n");
        return 0;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int level_counts[4] = {0};
    time_t earliest = logs[0].timestamp;
    time_t latest = logs[0].timestamp;
    
    for (int i = 0; i < log_count; i++) {
        level_counts[logs[i].level]++;
        
        if (logs[i].timestamp < earliest) {
            earliest = logs[i].timestamp;
        }
        if (logs[i].timestamp > latest) {
            latest = logs[i].timestamp;
        }
    }
    
    printf("Total logs: %d\n", log_count);
    printf("Time range: %ld to %ld\n", earliest, latest);
    printf("\nBreakdown by level:\n");
    
    for (enum LogLevel level = LOG_INFO; level <= LOG_CRITICAL; level++) {
        const char* level_name = level_to_string(level);
        int count = level_counts[level];
        float percentage = (float)count / log_count * 100.0f;
        printf("%-8s: %d (%.1f%%)\n", level_name, count, percentage);
    }
    
    printf("\nRecent critical events:\n");
    int critical_shown = 0;
    
    for (int i = 0; i < log_count; i++) {
        if (logs[i].level == LOG_CRITICAL) {
            printf("  [%ld] %s\n", logs[i].timestamp, logs[i].message);
            critical_shown++;
            if (critical_shown >= 5) {
                break;
            }
        }
    }
    
    if (critical_shown == 0) {
        printf("  No critical events found\n");
    }
    
    enum LogLevel most_common = LOG_INFO;
    int max_count = level_counts[LOG_INFO];
    
    for (enum LogLevel level = LOG_WARNING; level <= LOG_CRITICAL; level++) {
        if (level_counts[level] > max_count) {
            max_count = level_counts[level];
            most_common = level;
        }
    }
    
    printf("\nMost common log level: %s\n