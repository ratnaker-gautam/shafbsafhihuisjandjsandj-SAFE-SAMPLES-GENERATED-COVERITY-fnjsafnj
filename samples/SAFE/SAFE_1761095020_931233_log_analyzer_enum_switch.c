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
    
    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' to finish input\n");
    
    while (log_count < 100) {
        printf("Log %d: ", log_count + 1);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strcmp(buffer, "END") == 0) {
            break;
        }
        
        if (strlen(buffer) == 0) {
            continue;
        }
        
        char timestamp_str[64];
        char level_str[32];
        char message[256];
        
        int parsed = sscanf(buffer, "%63s %31s %255[^\n]", timestamp_str, level_str, message);
        if (parsed != 3) {
            printf("Invalid format. Use: TIMESTAMP LEVEL MESSAGE\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level: %s\n", level_str);
            continue;
        }
        
        struct tm tm_time;
        if (strptime(timestamp_str, "%Y-%m-%d_%H:%M:%S", &tm_time) == NULL) {
            printf("Invalid timestamp format. Use: YYYY-MM-DD_HH:MM:SS\n");
            continue;
        }
        
        logs[log_count].timestamp = mktime(&tm_time);
        logs[log_count].level = level;
        strncpy(logs[log_count].message, message, sizeof(logs[log_count].message) - 1);
        logs[log_count].message[sizeof(logs[log_count].message) - 1] = '\0';
        
        log_count++;
    }
    
    if (log_count == 0) {
        printf("No logs to analyze.\n");
        return 0;
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total logs: %d\n", log_count);
    
    int level_counts[4] = {0};
    time_t earliest = logs[0].timestamp;
    time_t latest = logs[0].timestamp;
    
    for (int i = 0; i < log_count; i++) {
        level_counts[logs[i].level]++;
        
        if (logs[i].timestamp < earliest) earliest = logs[i].timestamp;
        if (logs[i].timestamp > latest) latest = logs[i].timestamp;
    }
    
    printf("\nLog level distribution:\n");
    for (enum LogLevel level = LOG_INFO; level <= LOG_CRITICAL; level++) {
        printf("%s: %d\n", level_to_string(level), level_counts[level]);
    }
    
    printf("\nTime range: ");
    char time_buf[64];
    struct tm* tm_info = localtime(&earliest);
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("%s to ", time_buf);
    tm_info = localtime(&latest);
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("%s\n", time_buf);
    
    printf("\nCritical and Error logs:\n");
    for (int i = 0; i < log_count; i++) {
        switch (logs[i].level) {
            case LOG_ERROR:
            case LOG_CRITICAL:
                tm_info = localtime(&logs[i].timestamp);
                strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
                printf("[%s] %s: %s\n", time_buf, level_to_string(logs[i].level), logs[i].message);
                break;
            default:
                break;
        }
    }
    
    return 0;
}