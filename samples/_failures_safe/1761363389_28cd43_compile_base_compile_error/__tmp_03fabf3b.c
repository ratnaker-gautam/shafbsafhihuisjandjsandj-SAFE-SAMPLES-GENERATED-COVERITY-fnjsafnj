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

int main(void) {
    struct LogEntry logs[100];
    int log_count = 0;
    char input[512];
    char date_str[64];
    char time_str[64];
    char level_str[32];
    char message[256];
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE)\n");
    printf("Enter 'done' to finish input\n");
    
    while (log_count < 100) {
        printf("Log %d: ", log_count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strcspn(input, "\n");
        if (len < sizeof(input)) {
            input[len] = 0;
        }
        
        if (strcmp(input, "done") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        int parsed = sscanf(input, "%63s %63s %31s %255[^\n]", 
                           date_str, time_str, level_str, message);
        
        if (parsed != 4) {
            printf("Invalid format. Use: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE\n");
            continue;
        }
        
        char datetime_str[128];
        int fmt_len = snprintf(datetime_str, sizeof(datetime_str), "%s %s", date_str, time_str);
        if (fmt_len < 0 || fmt_len >= (int)sizeof(datetime_str)) {
            printf("Date/time string too long\n");
            continue;
        }
        
        struct tm tm = {0};
        char* result = strptime(datetime_str, "%Y-%m-%d %H:%M:%S", &tm);
        if (result == NULL || *result != '\0') {
            printf("Invalid date/time format\n");
            continue;
        }
        
        tm.tm_isdst = -1;
        time_t timestamp = mktime(&tm);
        if (timestamp == -1) {
            printf("Invalid date/time values\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level. Use: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
            continue;
        }
        
        logs[log_count].timestamp = timestamp;
        logs[log_count].level = level;
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
    
    int level_counts[5] = {0};
    time_t earliest = logs[0].timestamp;
    time_t latest = logs[0].timestamp;
    
    for (int i = 0; i < log_count; i++) {
        if (logs[i].level >= 0 && logs[i].level <= LOG_CRITICAL) {
            level_counts[logs[i].level]++;
        }
        
        if (logs[i].timestamp < earliest) {
            earliest = logs[i].timestamp;
        }
        if (logs[i].timestamp > latest) {
            latest = logs[i].timestamp;
        }
    }
    
    printf("Total logs: %d\n", log_count);
    printf("Time range: ");
    
    char time_buf[64];
    struct tm tm_early;
    struct tm* tm_info = localtime(&earliest);
    if (tm_info != NULL) {
        memcpy(&tm_early, tm_info, sizeof(struct tm));
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &tm_early);
        printf("%s to ", time_buf);
    }
    
    struct tm tm_late;
    tm_info = localtime(&latest);