//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum LogLevel {
    LOG_DEBUG,
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

void analyze_log_entry(const struct LogEntry* entry) {
    printf("Analyzing: [%s] ", level_to_string(entry->level));
    
    switch (entry->level) {
        case LOG_DEBUG:
            printf("Debug message - low priority\n");
            break;
        case LOG_INFO:
            printf("Informational - normal operation\n");
            break;
        case LOG_WARNING:
            printf("Warning detected - potential issue\n");
            break;
        case LOG_ERROR:
            printf("Error occurred - requires attention\n");
            break;
        case LOG_CRITICAL:
            printf("CRITICAL ALERT - immediate action needed\n");
            break;
        default:
            printf("Unknown log level\n");
            break;
    }
    
    char time_buf[64];
    struct tm time_info;
    struct tm* result = localtime_r(&entry->timestamp, &time_info);
    if (result != NULL) {
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &time_info);
        printf("Time: %s | Message: %s\n", time_buf, entry->message);
    } else {
        printf("Time: Invalid | Message: %s\n", entry->message);
    }
}

int main(void) {
    struct LogEntry logs[10];
    int log_count = 0;
    char input[512];
    
    printf("Log Analyzer - Enter up to 10 log entries\n");
    printf("Format: TIMESTAMP LEVEL MESSAGE\n");
    printf("Example: 1704067200 ERROR Database connection failed\n");
    printf("Levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n\n");
    
    while (log_count < 10) {
        printf("Log entry %d (or 'done' to finish): ", log_count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "done") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        char level_str[32];
        char message[256];
        long timestamp;
        
        int parsed = sscanf(input, "%ld %31s %255[^\n]", &timestamp, level_str, message);
        
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
            printf("Invalid log level. Use: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
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
        printf("No logs to analyze.\n");
        return 0;
    }
    
    printf("\n=== LOG ANALYSIS REPORT ===\n\n");
    
    int level_counts[5] = {0};
    
    for (int i = 0; i < log_count; i++) {
        analyze_log_entry(&logs[i]);
        
        switch (logs[i].level) {
            case LOG_DEBUG: level_counts[0]++; break;
            case LOG_INFO: level_counts[1]++; break;
            case LOG_WARNING: level_counts[2]++; break;
            case LOG_ERROR: level_counts[3]++; break;
            case LOG_CRITICAL: level_counts[4]++; break;
            default: break;
        }
    }
    
    printf("\n=== SUMMARY ===\n");