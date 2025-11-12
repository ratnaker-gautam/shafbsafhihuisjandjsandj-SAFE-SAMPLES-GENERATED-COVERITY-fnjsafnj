//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

struct LogEntry {
    char timestamp[20];
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

int validate_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int main(void) {
    struct LogEntry logs[100];
    int log_count = 0;
    char input[512];
    enum LogLevel filter_level = LOG_INFO;
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'FILTER LEVEL' to change filter, 'STATS' for statistics, 'QUIT' to exit\n");
    
    while (1) {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') input[len-1] = '\0';
        if (strlen(input) == 0) continue;
        
        if (strcmp(input, "QUIT") == 0) break;
        
        if (strcmp(input, "STATS") == 0) {
            int counts[5] = {0};
            for (int i = 0; i < log_count; i++) {
                if (logs[i].level >= 0 && logs[i].level <= LOG_CRITICAL) {
                    counts[logs[i].level]++;
                }
            }
            printf("Log Statistics:\n");
            for (int i = 0; i <= LOG_CRITICAL; i++) {
                printf("%s: %d\n", level_to_string(i), counts[i]);
            }
            continue;
        }
        
        if (strncmp(input, "FILTER ", 7) == 0) {
            char level_str[32];
            if (sscanf(input + 7, "%31s", level_str) == 1) {
                int new_level = parse_log_level(level_str);
                if (new_level != -1) {
                    filter_level = new_level;
                    printf("Filter level set to: %s\n", level_to_string(filter_level));
                } else {
                    printf("Invalid log level. Use: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
                }
            }
            continue;
        }
        
        if (log_count >= 100) {
            printf("Log storage full (max 100 entries)\n");
            continue;
        }
        
        char timestamp[20], level_str[32], message[256];
        if (sscanf(input, "%19s %31s %255[^\n]", timestamp, level_str, message) == 3) {
            if (!validate_timestamp(timestamp)) {
                printf("Invalid timestamp format. Use: YYYY-MM-DD HH:MM:SS\n");
                continue;
            }
            
            int level = parse_log_level(level_str);
            if (level == -1) {
                printf("Invalid log level. Use: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
                continue;
            }
            
            strncpy(logs[log_count].timestamp, timestamp, sizeof(logs[log_count].timestamp) - 1);
            logs[log_count].timestamp[sizeof(logs[log_count].timestamp) - 1] = '\0';
            logs[log_count].level = level;
            strncpy(logs[log_count].message, message, sizeof(logs[log_count].message) - 1);
            logs[log_count].message[sizeof(logs[log_count].message) - 1] = '\0';
            log_count++;
            
            if (level >= filter_level) {
                printf("[%s] %s: %s\n", timestamp, level_to_string(level),