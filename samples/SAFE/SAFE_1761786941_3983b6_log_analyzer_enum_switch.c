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
    char level_str[32];
    char message[256];
    int year, month, day, hour, minute, second;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE)\\n");
    printf("Enter 'END' on a new line to finish input\\n");
    
    while (log_count < 100) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        buffer[strcspn(buffer, "\\n")] = 0;
        
        if (strcmp(buffer, "END") == 0) {
            break;
        }
        
        if (sscanf(buffer, "%d-%d-%d %d:%d:%d %31s %255[^\\n]", 
                  &year, &month, &day, &hour, &minute, &second,
                  level_str, message) != 8) {
            printf("Invalid format. Skipping.\\n");
            continue;
        }
        
        if (year < 1970 || year > 2100 || month < 1 || month > 12 || 
            day < 1 || day > 31 || hour < 0 || hour > 23 || 
            minute < 0 || minute > 59 || second < 0 || second > 59) {
            printf("Invalid date/time. Skipping.\\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level. Skipping.\\n");
            continue;
        }
        
        struct tm time_struct = {0};
        time_struct.tm_year = year - 1900;
        time_struct.tm_mon = month - 1;
        time_struct.tm_mday = day;
        time_struct.tm_hour = hour;
        time_struct.tm_min = minute;
        time_struct.tm_sec = second;
        
        logs[log_count].timestamp = mktime(&time_struct);
        logs[log_count].level = level;
        strncpy(logs[log_count].message, message, sizeof(logs[log_count].message) - 1);
        logs[log_count].message[sizeof(logs[log_count].message) - 1] = '\\0';
        
        log_count++;
    }
    
    if (log_count == 0) {
        printf("No valid log entries entered.\\n");
        return 1;
    }
    
    printf("\\nLog Analysis Results:\\n");
    printf("=====================\\n");
    
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
    
    printf("Total entries: %d\\n", log_count);
    printf("Time range: %s", ctime(&earliest));
    printf("         to: %s", ctime(&latest));
    printf("\\nBreakdown by level:\\n");
    
    for (enum LogLevel level = LOG_INFO; level <= LOG_CRITICAL; level++) {
        printf("%-8s: %d entries\\n", level_to_string(level), level_counts[level]);
    }
    
    printf("\\nRecent critical events:\\n");
    int critical_shown = 0;
    for (int i = log_count - 1; i >= 0 && critical_shown < 3; i--) {
        if (logs[i].level == LOG_CRITICAL) {
            printf("- %s: %s\\n", ctime(&logs[i].timestamp), logs[i].message);
            critical_shown++;
        }
    }
    
    if (critical_shown == 0) {
        printf("No critical events found.\\n");
    }
    
    return 0;
}