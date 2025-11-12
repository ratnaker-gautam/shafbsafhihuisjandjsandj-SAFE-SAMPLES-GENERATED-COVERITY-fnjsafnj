//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG,
    LOG_UNKNOWN
};

struct LogEntry {
    char timestamp[20];
    enum LogLevel level;
    char message[256];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "DEBUG") == 0) return LOG_DEBUG;
    return LOG_UNKNOWN;
}

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_DEBUG: return "DEBUG";
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
    char line[300];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (log_count < 100) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        char timestamp[20];
        char level_str[10];
        char message[256];
        
        int parsed = sscanf(line, "%19s %9s %255[^\n]", timestamp, level_str, message);
        
        if (parsed != 3) {
            printf("Invalid log format. Skipping: %s\n", line);
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            printf("Invalid timestamp format. Skipping: %s\n", timestamp);
            continue;
        }
        
        enum LogLevel level = parse_log_level(level_str);
        if (level == LOG_UNKNOWN) {
            printf("Unknown log level. Skipping: %s\n", level_str);
            continue;
        }
        
        strncpy(logs[log_count].timestamp, timestamp, sizeof(logs[log_count].timestamp) - 1);
        logs[log_count].timestamp[sizeof(logs[log_count].timestamp) - 1] = '\0';
        logs[log_count].level = level;
        strncpy(logs[log_count].message, message, sizeof(logs[log_count].message) - 1);
        logs[log_count].message[sizeof(logs[log_count].message) - 1] = '\0';
        
        log_count++;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int level_counts[5] = {0};
    for (int i = 0; i < log_count; i++) {
        level_counts[logs[i].level]++;
    }
    
    for (int i = 0; i < 5; i++) {
        enum LogLevel current_level = i;
        printf("%s: %d entries\n", level_to_string(current_level), level_counts[i]);
    }
    
    printf("\nDetailed Logs:\n");
    for (int i = 0; i < log_count; i++) {
        switch (logs[i].level) {
            case LOG_ERROR:
                printf("[ERROR] %s: %s\n", logs[i].timestamp, logs[i].message);
                break;
            case LOG_WARNING:
                printf("[WARNING] %s: %s\n", logs[i].timestamp, logs[i].message);
                break;
            case LOG_INFO:
                printf("[INFO] %s: %s\n", logs[i].timestamp, logs[i].message);
                break;
            case LOG_DEBUG:
                printf("[DEBUG] %s: %s\n", logs[i].timestamp, logs[i].message);
                break;
            default:
                printf("[UNKNOWN] %s: %s\n", logs[i].timestamp, logs[i].message);
                break;
        }
    }
    
    return 0;
}