//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
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
            printf("Debug information - low priority\n");
            break;
        case LOG_INFO:
            printf("Normal operation - informational\n");
            break;
        case LOG_WARNING:
            printf("Potential issue detected - monitor closely\n");
            break;
        case LOG_ERROR:
            printf("Error occurred - requires attention\n");
            break;
        case LOG_CRITICAL:
            printf("Critical failure - immediate action required\n");
            break;
        default:
            printf("Unknown log level\n");
            break;
    }
    
    printf("Message: %s\n", entry->message);
    char time_buf[32];
    struct tm* tm_info = localtime(&entry->timestamp);
    if (tm_info != NULL) {
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
        printf("Timestamp: %s\n", time_buf);
    } else {
        printf("Timestamp: Invalid\n");
    }
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
    printf("Example: 2024-01-15 14:30:00 ERROR Database connection failed\n");
    printf("Available levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n\n");
    
    while (log_count < 10) {
        printf("Enter log entry %d (or 'quit' to finish): ", log_count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
        }
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        struct tm time_info = {0};
        int parsed = sscanf(input, "%d-%d-%d %d:%d:%d %31s %255[^\n]",
                           &year, &month, &day, &hour, &min, &sec,
                           level_str, message);
        
        if (parsed < 8) {
            printf("Invalid format. Please use: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE\n");
            continue;
        }
        
        if (year < 1900 || year > 2100 || month < 1 || month > 12 || day < 1 || day > 31 ||
            hour < 0 || hour > 23 || min < 0 || min > 59 || sec < 0 || sec > 59) {
            printf("Invalid date/time values\n");
            continue;
        }
        
        time_info.tm_year = year - 1900;
        time_info.tm_mon = month - 1;
        time_info.tm_mday = day;
        time_info.tm_hour = hour;
        time_info.tm_min = min;
        time_info.tm_sec = sec;
        time_info.tm_isdst = -1;
        
        logs[log_count].timestamp = mktime(&time_info);
        if (logs[log_count].timestamp == (time_t)-1) {
            printf("Invalid date/time\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level. Use: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
            continue;
        }
        logs[log_count].level = level;
        
        strncpy(logs[log_count].message, message, sizeof(logs[log_count].message) -