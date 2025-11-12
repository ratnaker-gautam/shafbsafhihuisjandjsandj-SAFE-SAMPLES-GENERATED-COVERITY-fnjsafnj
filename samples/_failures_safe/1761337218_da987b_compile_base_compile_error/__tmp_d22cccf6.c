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
    printf("Timestamp: %ld | Level: %s | Message: %s\n", 
           (long)entry->timestamp, level_to_string(entry->level), entry->message);
    
    switch (entry->level) {
        case LOG_DEBUG:
            printf("  Action: Debug information - no action required\n");
            break;
        case LOG_INFO:
            printf("  Action: Informational message - monitor if frequent\n");
            break;
        case LOG_WARNING:
            printf("  Action: Warning detected - investigate potential issues\n");
            break;
        case LOG_ERROR:
            printf("  Action: Error occurred - immediate attention required\n");
            break;
        case LOG_CRITICAL:
            printf("  Action: CRITICAL FAILURE - SYSTEM MAY BE UNSTABLE\n");
            break;
        default:
            printf("  Action: Unknown log level - verify log source\n");
            break;
    }
    printf("\n");
}

int main(void) {
    struct LogEntry logs[10];
    int log_count = 0;
    char input[512];
    char level_str[32];
    char message[256];
    int year, month, day, hour, min, sec;
    
    printf("Log Analyzer - Enter up to 10 log entries\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE\n");
    printf("Example: 2024-01-15 14:30:25 ERROR Database connection failed\n");
    printf("Available levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n\n");
    
    while (log_count < 10) {
        printf("Enter log entry %d (or 'quit' to finish): ", log_count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        int parsed = sscanf(input, "%d-%d-%d %d:%d:%d %31s %255[^\n]",
                           &year, &month, &day, &hour, &min, &sec, level_str, message);
        
        if (parsed < 7) {
            printf("Invalid format. Please use: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE\n");
            continue;
        }
        
        if (year < 2000 || year > 2100 || month < 1 || month > 12 || 
            day < 1 || day > 31 || hour < 0 || hour > 23 || 
            min < 0 || min > 59 || sec < 0 || sec > 59) {
            printf("Invalid date/time values\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level. Use: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
            continue;
        }
        
        struct tm time_struct = {0};
        time_struct.tm_year = year - 1900;
        time_struct.tm_mon = month - 1;
        time_struct.tm_mday = day;
        time_struct.tm_hour = hour;
        time_struct.tm_min = min;
        time_struct.tm_sec = sec;
        
        logs[log_count].timestamp = mktime(&time_struct);
        logs[log_count].level = level;
        strncpy(logs[log_count].message, message, sizeof(logs[log_count].message) - 1);
        logs[log_count].message[sizeof(logs[log_count].message) - 1] = '\0';
        
        log_count++;
    }
    
    if (log_count == 0) {
        printf("No log entries to analyze.\n");
        return 0;
    }
    
    printf("\n=== LOG ANALYSIS REPORT ===\n\n");
    
    int level_counts[5] = {0};
    time_t earliest = 0, latest = 0;