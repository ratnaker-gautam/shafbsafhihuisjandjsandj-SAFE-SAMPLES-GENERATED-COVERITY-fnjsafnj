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
    char buffer[512];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Example: 1609459200 INFO \"System started\"\n");
    printf("Enter 'done' to finish input\n");
    
    while (log_count < 100) {
        printf("Log %d: ", log_count + 1);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strcmp(buffer, "done") == 0) {
            break;
        }
        
        if (strlen(buffer) == 0) {
            continue;
        }
        
        char level_str[32];
        char message[256];
        long timestamp;
        
        int parsed = sscanf(buffer, "%ld %31s %255[^\n]", &timestamp, level_str, message);
        if (parsed != 3) {
            printf("Invalid format. Use: TIMESTAMP LEVEL MESSAGE\n");
            continue;
        }
        
        if (timestamp < 0) {
            printf("Invalid timestamp\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level. Use: INFO, WARNING, ERROR, CRITICAL\n");
            continue;
        }
        
        if (strlen(message) >= sizeof(logs[0].message)) {
            printf("Message too long\n");
            continue;
        }
        
        logs[log_count].timestamp = timestamp;
        logs[log_count].level = level;
        strncpy(logs[log_count].message, message, sizeof(logs[0].message) - 1);
        logs[log_count].message[sizeof(logs[0].message) - 1] = '\0';
        
        log_count++;
    }
    
    if (log_count == 0) {
        printf("No logs to analyze\n");
        return 0;
    }
    
    int level_counts[4] = {0};
    time_t earliest = logs[0].timestamp;
    time_t latest = logs[0].timestamp;
    
    for (int i = 0; i < log_count; i++) {
        if (logs[i].timestamp < earliest) earliest = logs[i].timestamp;
        if (logs[i].timestamp > latest) latest = logs[i].timestamp;
        
        switch (logs[i].level) {
            case LOG_INFO: level_counts[0]++; break;
            case LOG_WARNING: level_counts[1]++; break;
            case LOG_ERROR: level_counts[2]++; break;
            case LOG_CRITICAL: level_counts[3]++; break;
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", log_count);
    printf("Time range: %ld to %ld\n", earliest, latest);
    printf("Duration: %ld seconds\n", latest - earliest);
    printf("\nBreakdown by level:\n");
    
    for (int i = 0; i < 4; i++) {
        const char* level_name = level_to_string(i);
        int count = level_counts[i];
        double percentage = (double)count / log_count * 100.0;
        printf("%-8s: %d (%.1f%%)\n", level_name, count, percentage);
    }
    
    printf("\nRecent critical/error events:\n");
    int critical_shown = 0;
    
    for (int i = log_count - 1; i >= 0 && critical_shown < 5; i--) {
        if (logs[i].level == LOG_CRITICAL || logs[i].level == LOG_ERROR) {
            struct tm timeinfo;
            localtime_r(&logs[i].timestamp, &timeinfo);
            char time_buf[64];
            strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
            
            printf("[%s] %s: %s\n", time_buf, level_to_string(logs[i].level), logs[i].message);
            critical_shown++;
        }
    }
    
    if (critical_shown == 0) {
        printf("No critical or error events found\n");