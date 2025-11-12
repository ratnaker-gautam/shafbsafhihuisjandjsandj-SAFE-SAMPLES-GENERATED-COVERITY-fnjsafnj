//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

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

int validate_date(const char* date_str) {
    if (strlen(date_str) != 10) return 0;
    if (date_str[4] != '-' || date_str[7] != '-') return 0;
    
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date_str[i])) return 0;
    }
    
    int year, month, day;
    if (sscanf(date_str, "%d-%d-%d", &year, &month, &day) != 3) return 0;
    if (year < 2000 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    
    return 1;
}

int main(void) {
    struct LogEntry logs[100];
    int log_count = 0;
    char input[512];
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD LEVEL MESSAGE)\n");
    printf("Enter 'ANALYZE' to process logs or 'QUIT' to exit\n");
    printf("Supported levels: INFO, WARNING, ERROR, CRITICAL\n\n");
    
    while (log_count < 100) {
        printf("Log %d: ", log_count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "QUIT") == 0) {
            break;
        }
        
        if (strcmp(input, "ANALYZE") == 0) {
            break;
        }
        
        char date_str[11];
        char level_str[16];
        char message[256];
        
        if (sscanf(input, "%10s %15s %255[^\n]", date_str, level_str, message) != 3) {
            printf("Invalid format. Use: YYYY-MM-DD LEVEL MESSAGE\n");
            continue;
        }
        
        if (!validate_date(date_str)) {
            printf("Invalid date format. Use YYYY-MM-DD\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level. Use: INFO, WARNING, ERROR, CRITICAL\n");
            continue;
        }
        
        struct tm time_struct = {0};
        time_struct.tm_year = atoi(date_str) - 1900;
        time_struct.tm_mon = atoi(date_str + 5) - 1;
        time_struct.tm_mday = atoi(date_str + 8);
        time_struct.tm_isdst = -1;
        
        logs[log_count].timestamp = mktime(&time_struct);
        if (logs[log_count].timestamp == -1) {
            printf("Failed to parse date\n");
            continue;
        }
        logs[log_count].level = level;
        strncpy(logs[log_count].message, message, sizeof(logs[log_count].message) - 1);
        logs[log_count].message[sizeof(logs[log_count].message) - 1] = '\0';
        
        log_count++;
    }
    
    if (log_count == 0) {
        printf("No logs to analyze.\n");
        return 0;
    }
    
    printf("\n=== LOG ANALYSIS REPORT ===\n");
    printf("Total logs: %d\n\n", log_count);
    
    int level_counts[4] = {0};
    for (int i = 0; i < log_count; i++) {
        if (logs[i].level >= LOG_INFO && logs[i].level <= LOG_CRITICAL) {
            level_counts[logs[i].level]++;
        }
    }
    
    printf("Log Level Distribution:\n");
    for (enum LogLevel level = LOG_INFO; level <= LOG_CRITICAL; level++) {
        const char* level_name = level_to_string(level);
        printf("%-8s: %d (%.1f%%)\n", level_name, level_counts[level], 
               (float)level_counts[level] / log_count *